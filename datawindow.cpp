#include "datawindow.h"
#include "backtracewindow.h"
#include "gdb.h"
#include <cstdio>
#include <QMenu>
#include <QContextMenuEvent>
#include <QScrollBar>

int maxLevels = 10;

extern GDB *gdb;
extern BackTraceWindow *backTraceWindow;
extern IntHash sizeForType;
extern QSet<QString> simpleTypes;
extern DataWindow *dataWindow;
extern IntHash formatToSpan;
extern IntHash formatToSize;
extern QHash<QString,FormatFunction> formatToFunction;

extern DataPlank *latestPlank;

DataTree *dataTree;
DataPlank *globals;
DataPlank *locals;
DataPlank *parameters;
DataPlank *userDefined;
DataMap *localMap;
DataMap *parameterMap;
DataMap *globalMap;
DataMap *userDefinedMap;
StringHash formatForType;
QHash<DataMap*,DataPlank*> plankForMap;

extern QHash<QString, Range> labelRange;
QStack<DataTree*> stack;

extern StringHash varToAddress;

QHash<QString, ClassDefinition> classes;

DataWindow::DataWindow(QWidget *parent)
    : QFrame(parent)
{
    setObjectName("Data");
    setFrameStyle(QFrame::Panel | QFrame::Raised);
    setLineWidth(4);

    layout = new QVBoxLayout();
    layout->setContentsMargins(10, 10, 10, 10);

    dataTree = new DataTree(this);
    //dataTree->setShowGrid(false);
    scrollArea = new QScrollArea;
    scrollArea->setWidget(dataTree);
    layout->addWidget(scrollArea);
    dataTree->setToolTip(tr("Left click on a > symbol to expand a variable.\n"
                "Right click to change formats."));

    treeLevel = 1;

    setLayout(layout);

    qRegisterMetaType<DataMap*>("DataMap*");
    qRegisterMetaType<DataMap> ("DataMap");
    qRegisterMetaType<VariableDefinitionMap> ("VariableDefinitionMap");

    connect(gdb, SIGNAL(sendGlobals(VariableDefinitionMap)), this,
            SLOT(receiveGlobals(VariableDefinitionMap)));
    connect(gdb, SIGNAL(sendLocals(VariableDefinitionMap)), this,
            SLOT(receiveLocals(VariableDefinitionMap)));
    connect(gdb, SIGNAL(sendParameters(VariableDefinitionMap)), this,
            SLOT(receiveParameters(VariableDefinitionMap)));
    connect ( this,
            SIGNAL(requestVar(DataMap*,QString,QString,QString,QString,int)),
            gdb, SLOT(requestVar(DataMap*,QString,QString,QString,QString,int)) );
    connect ( gdb, SIGNAL(sendVar(DataMap*,QString,QString)),
            this, SLOT(receiveVar(DataMap*,QString,QString)) );
    //connect ( gdb, SIGNAL(dataReady(QStringList)),
    //this, SLOT(setData(QStringList)) );
    connect(gdb, SIGNAL(resetData()), this, SLOT(resetData()));
    connect(gdb, SIGNAL(sendClasses(QHash<QString, ClassDefinition>)), this,
            SLOT(receiveClasses(QHash<QString, ClassDefinition>)));
}

void DataWindow::saveScroll()
{
    QScrollBar *b = scrollArea->horizontalScrollBar();
    xScroll = b->value();
    b = scrollArea->verticalScrollBar();
    yScroll = b->value();
    //qDebug() << "Scroll" << xScroll << yScroll;
}

void DataWindow::restoreScroll()
{
    //qDebug() << "Setting Scroll" << xScroll << yScroll;
    QScrollBar *b = scrollArea->horizontalScrollBar();
    b->setValue(0);
    b->setValue(xScroll);
    b = scrollArea->verticalScrollBar();
    b->setValue(0);
    b->setValue(yScroll);
    //scrollArea->scroll(xScroll,yScroll);
}

QSize DataWindow::sizeHint() const
{
    return QSize(200, 10);
}

void DataWindow::setFontHeightAndWidth(int height, int width)
{
    //qDebug() << "setFontHeightAndWidth" << height << width;
    fontHeight = height;
    fontWidth = width;
    dataTree->fontWidth = width;
    dataTree->fontHeight = height;
    //dataTree->buildTree(dataTree->all);
    dataTree->setFontHeightAndWidth(height,width);
    dataTree->table.clear();
    dataTree->reorder(dataTree->all);
    dataTree->resizeToFitContents(maxLevels);
}

void DataWindow::receiveVariableDefinition(QStringList strings)
{
    DataPlank *plank;
    //qDebug() << "data rec var" << strings;
    QString name = strings[0];
    int line = strings[7].toInt();
    Range range;
    if (name[0] == QChar('.')) {
        foreach ( QString label, labelRange.keys() ) {
            range = labelRange[label];
            //qDebug() << label << range.first << range.last;
            if ( line >= range.first && line <= range.last &&
                    varToAddress.contains(label+name) ) {
                name = label+name;
                break;
            }
        }
    }
    plank = userDefinedMap->value(name);
    if (plank == 0) {
        //plank = dataTree->addDataPlank(userDefined,userDefinedMap, name, strings[2]);
    }
    plank->address = strings[1];
    if (varToAddress.contains(name)) {
        plank->address = varToAddress[name];
    }
    if (plank->address == "") {
        plank->address = QString("&(%1)").arg(name);
    }
    if (strings[3] == "string" && strings[5] == "0") strings[5] = "1";
    plank->setRange(strings[4].toInt(), strings[5].toInt());
    plank->setType(strings[2]);
    plank->format = strings[3];
    userDefined->addChild(plank);
    userDefined->sortChildren();
    request(plank);
}

void DataWindow::resetData()
{
    QString s;
    dataWindow->saveScroll();
    int n = userDefined->kids.length();
    //qDebug() << "reset" << n;
    dataTree->buildTree(dataTree->all);
    dataTree->table.clear();
    dataTree->reorder(dataTree->all);
    dataTree->resizeToFitContents(maxLevels);
    dataWindow->restoreScroll();
    for (int i = 0; i < n; i++) {
        request(userDefined->kids[i]);
    }
}

void DataWindow::receiveClasses(QHash<QString, ClassDefinition> c)
{
    //qDebug() << "receive classes" << c.keys();
    classes = c;
}

DataPlank::DataPlank(QWidget *p)
    : EZPlank(p)
{
    name = "";
    type = "";
    stringValue = "";
    values = 0;
    a.u8 = 0;
    first = 0;
    last = 0;
    userDefined = false;
    kidCount = 0;
    format = "";
    //setFlags(Qt::ItemIsSelectree | Qt::ItemIsEnabled);
}

void DataPlank::sortChildren()
{
}

QString DataPlank::value()
{
    QString val;
    if (isSimple && last == 0) {
        switch (size) {
            case 1:
                if (format == "Character")
                    if ( isprint(a.c) ) {
                        val.sprintf("%c", a.c);
                    } else if ( a.c == '\t' ) {
                        val = "\\t";
                    } else if ( a.c == '\n' ) {
                        val = "\\n";
                    } else if ( a.c == '\r' ) {
                        val = "\\r";
                    } else {
                        val.sprintf("0x%x", a.u1);
                    }
                    else if (format == "Decimal")
                        val.sprintf("%d", a.i1);
                    else if (format == "Unsigned decimal")
                        val.sprintf("%d", a.u1);
                    else if (format == "Hexadecimal")
                        val.sprintf("0x%x", a.u1);
                    else if (format == "Boolean")
                        val = a.b1 ? "true" : "false";
                    else if (format == "Binary")
                        val = binary(a, 1);
                    else
                        val = "";
                break;
            case 2:
                if (format == "Decimal")
                    val.sprintf("%d", a.i2);
                else if (format == "Unsigned decimal")
                    val.sprintf("%d", a.u2);
                else if (format == "Hexadecimal")
                    val.sprintf("0x%x", a.u2);
                else if (format == "Boolean")
                    val = a.b1 ? "true" : "false";
                else if (format == "Binary")
                    val = binary(a, 2);
                else
                    val = "";
                break;
            case 4:
                if (format == "Decimal")
                    val.sprintf("%d", a.i4);
                else if (format == "Unsigned decimal")
                    val.sprintf("%d", a.u4);
                else if (format == "Hexadecimal")
                    val.sprintf("0x%x", a.u4);
                else if (format == "Floating point")
                    val.sprintf("%g", a.f4);
                else if (format == "Boolean")
                    val = a.b1 ? "true" : "false";
                else if (format == "Binary")
                    val = binary(a, 4);
                else if (format == "Binary fp")
                    val = binaryFloat(a);
                else if (format == "Fields")
                    val = fieldsFloat(a);
                else
                    val = "";
                break;
            case 8:
#ifdef Q_WS_WIN
                if ( format == "Decimal" ) val.sprintf("%lld",a.i8);
                else if ( format == "Unsigned decimal" ) val.sprintf("%llu",a.u8);
                else if ( format == "Hexadecimal" ) val.sprintf("0x%llx",a.u8);
#else
                if (format == "Decimal")
                    val.sprintf("%ld", a.i8);
                else if (format == "Unsigned decimal")
                    val.sprintf("%ld", a.u8);
                else if (format == "Hexadecimal")
                    val.sprintf("0x%lx", a.u8);
#endif
                else if (format == "Floating point")
                    val.sprintf("%g", a.f8);
                else if (format == "Boolean")
                    val = a.b1 ? "true" : "false";
                else if (format == "Binary")
                    val = binary(a, 8);
                else if (format == "Binary fp")
                    val = binaryDouble(a);
                else if (format == "Fields")
                    val = fieldsDouble(a);
                else
                    val = "";
                break;
            default:
                val = "";
        }
    } else if (stringValue.indexOf("Illegal") >= 0) {
        val = stringValue;
    } else if (classes.contains(type)) {
        val = "";
    } else if (format == "string" ) {
        val = stringValue;
        val.replace("\"","");
    } else {
        val = stringValue;
    }
    //qDebug() << "value" << name << format << val;
    //qDebug() << "isSimple" << isSimple << ",  size" << size << ",  last"
    //<< last;
    return val;
}

QString DataPlank::valueFromGdb()
{
    return "";
}

void DataPlank::setName(QString n)
{
    //qDebug() << "setName" << n;
    //qDebug() << "level" << treeLevel;
    QString shortName;
    int k = n.lastIndexOf('.');
    //qDebug() << "k" << k;
    tree->setCurrentPlank(this);
    
    if (k >= 0)
        shortName = n.mid(k + 1);
    else
        shortName = n;
    name = shortName;
    //qDebug() << "shortName" << shortName;
    if ( rows < 1 ) tree->setRowCount(1);
    if ( treeLevel > 0 ) {                       // Assume treeLevel is 1
        int i;
        for ( i=0; i < treeLevel-1; i++ ) {
            tree->setSpan(0,i,1,1);              // Empty space before indicator
            tree->setText(0,i," ",EZ::Default);  // size treeLevel - 1
        }
        tree->setSpan ( 0, i, 1, 1 );            // Column 0 spans 1 column
        tree->setText(0,i,"-",EZ::Default);      // Indicator in 0
    }
    if ( treeLevel <= 0 ) {
        tree->setSpan(0,0,1,maxLevels+1);
        tree->setText(0,0," "+shortName,EZ::Default);// Name in 0 - 10
    } else {
        tree->setSpan(0,treeLevel,1,maxLevels-treeLevel+1);
        tree->setText(0,treeLevel,shortName,EZ::Default);  // Name in 1 - 10
    }
    //if (first == 0 && last == 0) {
        //tree->setText(0, treeLevel, shortName);
    //} else {
        //tree->setText(0, treeLevel,
                //QString("%1[%2:%3]").arg(shortName).arg(first).arg(last));
    //}
}

void DataPlank::setType(QString t)
{
    //qDebug() << "setType" << t << type << basicType << format;
    if ( t == type ) return;

    type = t;
    int n=t.indexOf("[");
    if ( n < 0 ) n = t.indexOf("(");
    if ( n < 0 ) n = t.indexOf("*");
    if ( n >= 0 ) {
        basicType = t.left(n).trimmed();
    } else {
        basicType = t;
    }

    if (sizeForType.contains(t)) {
        size = sizeForType[t];
    } else {
        size = 8;
    }

    isSimple = simpleTypes.contains(t);
    if (isSimple) {
        basicType = type;
        if ( formatForType.contains(t) ) {
            format = formatForType[t];
        } else {
            format = QString("hex%1").arg(size);
        }
    } else if (t == "char **") {
        format = "string array";
    } else if (t == "char *" || t == "std::string" ) {
        format = "string";
    } else if (t.indexOf(" *") >= 0) {
        format = QString("hex%1").arg(size);
    } else if ( formatForType.contains(t) ) {
        format = formatForType[t];
    } else if ( formatForType.contains(basicType) ) {
        format = formatForType[basicType];
    } else if ( sizeForType.contains(basicType) ) {
        size = sizeForType[basicType];
        format = QString("hex%1").arg(size);
    } else {
        format = "hex1";
    }

    tree->setCurrentPlank(this);
    if ( rows < 1 ) tree->setRowCount(1);
    tree->setSpan ( 0, maxLevels+1, 1, 1 );          // 11 spans 1
    tree->setText (0,maxLevels+1,type,EZ::Default);  // Type in 11
    //if (isSimple && last == 0) {
    //setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicator);
    //} else {
    //setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
    //}
}

void DataPlank::setValues ( QStringList s)
{
    QString t;
    QStringList parts;
    bool ok;
    int k = 0;

    //qDebug() << "setValues" << s;
    if ( format == "string" || format == "string array" )  {
        stringValues = s;
    } else {
        for ( int j = 0; j < s.size(); j++ ) {
            t = s[j];
            parts = t.split(":");
            if ( parts.size() > 1 ) {
                t = parts[1];
            }
            t.replace(",","");
            t.replace("(","");
            t.replace(")","");
            t.replace("0x","");
            t.replace("\t"," ");
            t.replace("  "," ");
            t.remove(0,1);
            parts = t.split(" ");
            //qDebug() << "setValues" << parts;
            int n = parts.length();
            if ( n > size-k ) n = size-k;
            for ( int r=0; r < n; r++ ) {
                values->u1(k) = parts[r].toInt(&ok,16);
                k++;
            }
        }
    }
    size = k;
}

void DataPlank::setValue(QString v)
{
    bool ok;

    stringValue = v;
    //qDebug() << name << size << v << a.u8 << value() << v;
    if (isSimple && last == 0) {
        a.u8 = 0;
        switch (size) {
            case 1:
                a.u1 = v.toUInt(&ok, 16);
                break;
            case 2:
                a.u2 = v.toUInt(&ok, 16);
                break;
            case 4:
                a.u4 = v.toUInt(&ok, 16);
                break;
            case 8:
#ifdef Q_WS_WIN
                a.u8 = v.toULongLong(&ok,16);
#else
                a.u8 = v.toULong(&ok, 16);
#endif
                break;
        }
        //qDebug() << name << size << v << a.u8 << value();
    }
    tree->setCurrentPlank(this);
    tree->setSpan(0, maxLevels+2, 1, tree->columns );
    tree->setText(0, maxLevels+2, value());
}

void DataPlank::setRange(int f, int l)
{
    first = f;
    last = l;
    setName(name);
}

int DataPlank::maxLevel()
{
    int max = treeLevel;
    int kidMax;
    if ( state == EZ::Expanded ) {
        DataPlank *kid;
        for ( int i=0; i < kids.length(); i++ ) {
            kid = (DataPlank *)kids[i];
            kidMax = kid->maxLevel();
            if ( kidMax > max ) max = kidMax;
        }
    }
    return max;
}

void DataTree::setBinary()
{
    DataPlank *d = (DataPlank *)latestPlank;
    d->format = QString("bin%1").arg(sizeForType[d->basicType]);
    dataWindow->saveScroll();
    buildTree(all);
    table.clear();
    reorder(all);
    resizeToFitContents(maxLevels);
    dataWindow->restoreScroll();
}

void DataTree::setBool()
{
    DataPlank *d = (DataPlank *)latestPlank;
    d->format = QString("bool%1").arg(sizeForType[d->basicType]);
    dataWindow->saveScroll();
    buildTree(all);
    table.clear();
    reorder(all);
    resizeToFitContents(maxLevels);
    dataWindow->restoreScroll();
}

void DataTree::setDecimal()
{
    DataPlank *d = (DataPlank *)latestPlank;
    d->format = QString("dec%1").arg(sizeForType[d->basicType]);
    dataWindow->saveScroll();
    buildTree(all);
    table.clear();
    reorder(all);
    resizeToFitContents(maxLevels);
    dataWindow->restoreScroll();
}

void DataTree::setUnsignedDecimal()
{
    DataPlank *d = (DataPlank *)latestPlank;
    d->format = QString("udec%1").arg(sizeForType[d->basicType]);
    dataWindow->saveScroll();
    buildTree(all);
    table.clear();
    reorder(all);
    resizeToFitContents(maxLevels);
    dataWindow->restoreScroll();
}

void DataTree::setHexadecimal()
{
    DataPlank *d = (DataPlank *)latestPlank;
    d->format = QString("hex%1").arg(sizeForType[d->basicType]);
    dataWindow->saveScroll();
    buildTree(all);
    table.clear();
    reorder(all);
    resizeToFitContents(maxLevels);
    dataWindow->restoreScroll();
}

void DataTree::setCharacter()
{
    DataPlank *d = (DataPlank *)latestPlank;
    d->format = "char";
    dataWindow->saveScroll();
    buildTree(all);
    table.clear();
    reorder(all);
    resizeToFitContents(maxLevels);
    dataWindow->restoreScroll();
}

void DataTree::setFloatingPoint()
{
    DataPlank *d = (DataPlank *)latestPlank;
    d->format = sizeForType[d->basicType] == 4 ? "float" : "double";
    dataWindow->saveScroll();
    buildTree(all);
    table.clear();
    reorder(all);
    resizeToFitContents(maxLevels);
    dataWindow->restoreScroll();
}

void DataTree::setBinaryFP()
{
    DataPlank *d = (DataPlank *)latestPlank;
    d->format = QString("binaryfp%1").arg(sizeForType[d->basicType]);
    dataWindow->saveScroll();
    buildTree(all);
    table.clear();
    reorder(all);
    resizeToFitContents(maxLevels);
    dataWindow->restoreScroll();
}

void DataTree::setFields()
{
    DataPlank *d = (DataPlank *)latestPlank;
    d->format = QString("fields%1").arg(sizeForType[d->basicType]);
    dataWindow->saveScroll();
    buildTree(all);
    table.clear();
    reorder(all);
    resizeToFitContents(maxLevels);
    dataWindow->restoreScroll();
}

void DataTree::editUserVariable()
{
    DataPlank *d = (DataPlank *)latestPlank;
    DefineVariableDialog *dialog = new DefineVariableDialog;
    dialog->nameEdit->setText(d->name);
    dialog->addressEdit->setText(d->address);
    dialog->formatCombo->setCurrentIndex(
            dialog->formatCombo->findText(d->format));
    dialog->typeCombo->setCurrentIndex(
            dialog->typeCombo->findText(QString("%1").arg(d->type)));
    dialog->firstEdit->setText(QString("%1").arg(d->first));
    dialog->lastEdit->setText(QString("%1").arg(d->last));
    if (dialog->exec()) {
        userDefinedMap->remove(d->name);
        d->setName(dialog->nameEdit->text());
        d->address = dialog->addressEdit->text();
        d->first = dialog->firstEdit->text().toInt();
        d->last = dialog->lastEdit->text().toInt();
        d->setType(dialog->typeCombo->currentText());
        d->format = dialog->formatCombo->currentText();
        userDefinedMap->insert(d->name, d);
        setText(0, maxLevels+2, d->value());
    }
    delete dialog;
}

void DataTree::deleteUserVariable()
{
    DataPlank *plank = (DataPlank *)latestPlank;
    for ( int i=0; i < kids.length(); i++ ) {
        {
            kids[i]->removeSubTree();
            delete kids[i];
        }
        for ( int i=0; i < plank->parent->kids.length(); i++ ) {
            if ( (DataPlank*)(plank->parent->kids[i]) == plank ) {
                plank->parent->kids.removeAt(i);
                break;
            }
        }
    }
    userDefinedMap->remove(plank->name);
    delete plank;
}

void DataTree::contextMenuEvent(QContextMenuEvent * /*event*/)
{
    DataPlank *p= (DataPlank *)latestPlank;
    QString type = p->type;
    //qDebug() << p->name << type << p->value();
    if (p->userDefined) {
        QMenu menu(tr("Variable menu"));
        menu.addAction(tr("Edit variable"), this, SLOT(editUserVariable()));
        menu.addAction(tr("Delete variable"), this, SLOT(deleteUserVariable()));
        menu.exec(QCursor::pos());
    } else if (type.indexOf("char") >= 0) {
        QMenu menu(tr("Character menu"));
        menu.addAction(tr("Character"), this, SLOT(setCharacter()));
        menu.addAction(tr("Decimal"), this, SLOT(setDecimal()));
        menu.addAction(tr("Unsigned decimal"), this,
                SLOT(setUnsignedDecimal()));
        menu.addAction(tr("Hexadecimal"), this, SLOT(setHexadecimal()));
        menu.addAction(tr("Binary"), this, SLOT(setBinary()));
        menu.exec(QCursor::pos());
    } else if (type.indexOf("short") >= 0 || type.indexOf("int") >= 0
            || type.indexOf("long") >= 0) {
        QMenu menu(tr("Integer menu"));
        menu.addAction(tr("Signed decimal"), this, SLOT(setDecimal()));
        menu.addAction(tr("Unsigned decimal"), this,
                SLOT(setUnsignedDecimal()));
        menu.addAction(tr("Hexadecimal"), this, SLOT(setHexadecimal()));
        menu.addAction(tr("Binary"), this, SLOT(setBinary()));
        menu.exec(QCursor::pos());
    } else if (type == "float" || type == "double"
            || type.indexOf("real") >= 0) {
        QMenu menu(tr("Float menu"));
        menu.addAction(tr("Floating point"), this, SLOT(setFloatingPoint()));
        menu.addAction(tr("Hexadecimal"), this, SLOT(setHexadecimal()));
        menu.addAction(tr("Binary"), this, SLOT(setBinary()));
        menu.addAction(tr("Binary fp"), this, SLOT(setBinaryFP()));
        menu.addAction(tr("Fields"), this, SLOT(setFields()));
        menu.exec(QCursor::pos());
    } else if (type == "bool" || type.indexOf("logical") >= 0) {
        QMenu menu("Boolean menu");
        menu.addAction(tr("Boolean"), this, SLOT(setBool()));
        menu.addAction(tr("Binary"), this, SLOT(setBinary()));
        menu.exec(QCursor::pos());
    }
}

void DataTree::setPlankCount(int n )
{
    //qDebug() << "setPlankCount" << planks << n;
    for ( int p=0; p < planks; p++ ) {
        if ( table[p] ) {
            table[p]->hide();
            delete table[p];
        }
    }

    table.resize(n);
    DataPlank *plank;
    for ( int p=0; p < n; p++ ) {
        table[p] = plank = new DataPlank(this);
        plank->show();
    }
    planks = n;
}


DataTree::DataTree(QWidget *parent)
: EZTable(parent)
{
    //DataPlank *stack;
    columns = ebe["data/columns"].toInt();
    setPlankCount(0);
    setColumnCount(columns+maxLevels+2);

    //horizontalHeader()->show();
    ::globalMap = globalMap = new DataMap;
    ::localMap = localMap = new DataMap;
    ::parameterMap = parameterMap = new DataMap;
    ::userDefinedMap = userDefinedMap = new DataMap;

    formatForType["bool"] = "bool1";
    formatForType["char"] = "char";
    formatForType["signed char"] = "dec1";
    formatForType["unsigned char"] = "hex1";
    formatForType["short"] = "dec2";
    formatForType["signed short"] = "dec2";
    formatForType["unsigned short"] = "hex2";
    formatForType["int"] = "dec4";
    formatForType["signed int"] = "dec4";
    formatForType["unsigned int"] = "hex4";
#ifdef Q_WS_WIN
    formatForType["long"] = "dec4";
    formatForType["signed long"] = "dec4";
    formatForType["unsigned long"] = "hex4";
#else
    formatForType["long"] = "dec8";
    formatForType["signed long"] = "dec8";
    formatForType["unsigned long"] = "hex8";
#endif
    formatForType["long long"] = "dec8";
    formatForType["signed long long"] = "dec8";
    formatForType["unsigned long long"] = "hex8";
    formatForType["float"] = "float";
    formatForType["double"] = "double";
    formatForType["character"] = "char";
    formatForType["logical(kind=1)"] = "bool1";
    formatForType["logical(kind=2)"] = "bool2";
    formatForType["logical(kind=4)"] = "bool4";
    formatForType["logical(kind=8)"] = "bool8";
    formatForType["integer(kind=2)"] = "dec2";
    formatForType["integer(kind=4)"] = "dec4";
    formatForType["integer(kind=8)"] = "dec8";
    formatForType["real(kind=4)"] = "float";
    formatForType["real(kind=8)"] = "double";
    fontWidth =  ebe["font_size"].toInt()*.6666;
    fontHeight = fontWidth * 2;
    ezrowHeight = 1.1 * fontHeight;
    //all = new DataPlank;
    all = addDataPlank(0,0,globalMap, tr("Name"), tr("Type"));
    all->format = "string";
    all->state = EZ::Simple;
    setSpan(0,maxLevels+2,1,columns);
    setText(0,maxLevels+2,tr("Value"));
    all->stringValues.append(tr("Value"));

    ::globals = globals = addDataPlank(all,1,globalMap, tr("globals"), "");
    ::locals = locals = addDataPlank(all,1,localMap, tr("locals"), "");
    ::parameters = parameters = addDataPlank(all,1,parameterMap,
            tr("parameters"), "");
    ::userDefined = userDefined = addDataPlank(all,1,userDefinedMap,
            tr("user-defined"), "");
    globalMap->remove(tr("Name"));
    globalMap->remove(tr("globals"));
    localMap->remove(tr("locals"));
    parameterMap->remove(tr("parameters"));
    userDefinedMap->remove(tr("user-defined"));
    plankForMap[globalMap] = globals;
    plankForMap[localMap] = locals;
    plankForMap[parameterMap] = parameters;
    plankForMap[userDefinedMap] = userDefined;

    //globals->setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicator);
    globals->state = EZ::Expanded;
    globals->parent = 0;
    //locals->setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicator);
    locals->state = EZ::Expanded;
    locals->parent = 0;
    //parameters->setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicator);
    parameters->parent = 0;
    parameters->state = EZ::Expanded;
    //userDefined->setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicator);
    userDefined->parent = 0;
    userDefined->state = EZ::Expanded;
    for ( int c=0; c < columns+2; c++ ) {
        setColumnWidth(c, fontWidth*0.8);
    }

    //connect ( this, SIGNAL(plankExpanded(QTreeWidgetItem*)),
    //this, SLOT(expandDataPlank(QTreeWidgetItem*)) );
    //connect ( this, SIGNAL(plankCollapsed(QTreeWidgetItem*)),
    //this, SLOT(collapseDataPlank(QTreeWidgetItem*)) );
    buildTree(all);
    //resizeToFitContents(maxLevels);
    //qDebug() << "Done";
}

void DataTree::reorder(DataPlank *p)
{
    table.append(p);
    for ( int j=0; j < p->kids.length(); j++ ) {
        reorder(p->kids[j]);
    }
}

void DataTree::buildTree(DataPlank *p)
{
    //qDebug() << "buildTree" << p << p->name << p->format;
    setCurrentPlank(p);
    redisplay(p, EZ::Highlight);
    for ( int j=0; j < p->kids.length(); j++ ) {
        buildTree(p->kids[j]);
    }
}
/*
   setRowCount(1);
   setText(0,0,k->name,EZ::Default);
   setText(0,1,k->type,EZ::Default);
   if ( k->treeLevel == 0 ) {
   setSpan(0,2,1,columns);
   setText ( 0, 2, "", EZ::Default );
   } else {
   if ( simpleTypes.contains(k->type) ||
   k->format == "string" ||
   k->type.indexOf("character*") >= 0 ) {
   setSpan ( nextRow, 2, 1, columns );
   setText ( nextRow, 2, k->value(), EZ::Highlight );
   } else if ( k->format == "String array" ) {
   QString s = k->stringValue;
   QStringList parts;
   int len = s.length();
   s.remove(len-2,2);
   s.remove(0,1);
   parts = s.split("\" \"");
   setRowCount(parts.length());
   nextRow=0;
   foreach ( QString t, parts ) {
   setSpan ( nextRow, 2, 1, columns );
   setText ( nextRow, 2, t, EZ::Highlight );
   nextRow++;
   setText ( nextRow, 0, "", EZ::Default );
   setText ( nextRow, 1, "", EZ::Default );
   }
   } else if ( k->dimensions.size() == 1 &&
   formatToSpan[k->basicType] > 0 ) {
   int span = formatToSpan[k->basicType];
   int count = columns/span;
   QStringList parts = k->stringValue.split(" ");
   int n = parts.length();
   int numRows = (n+count-1)/count;
   int k = 0;
   int m;
   int left = n;
   setRowCount(numRows);
   for ( int r = 0; r < numRows; r++ ) {
   if ( r > 0 ) {
   setText ( r, 0, "", EZ::Default );
   setText ( r, 1, "", EZ::Default );
   }
   m = left;
   if ( m > count ) m = count;
   qDebug() << span << count << numRows << r << left << n;
   for ( k = 0; k < m; k++ ) {
   if ( r == numRows - 1 && k == n-1 ) {
   setSpan ( r, k+2, 1, columns-k);
   } else {
   setSpan ( r, k+2, 1, span );
   }
   setText ( r, k+2, parts[k+r*count], EZ::Highlight );
   }
   left -= m;
   }
   } else if ( 
   */

DataTree::~DataTree()
{
    globals->removeSubTree();
    delete globals;
    locals->removeSubTree();
    delete locals;
    parameters->removeSubTree();
    delete parameters;
    userDefined->removeSubTree();
    delete userDefined;
    delete globalMap;
    delete localMap;
    delete parameterMap;
    delete userDefinedMap;
}

void DataTree::collapseDataPlank(DataPlank *plank)
{
    DataPlank *it = (DataPlank *)plank;
    if ( plank->root >= 0) return;
    it->state = EZ::Collapsed;
}

void DataTree::expandDataPlank(DataPlank *plank)
{
    DataPlank *it = (DataPlank *)plank;
    DataPlank *d;
    QString fullName;
    ClassDefinition c;
    int n;

    c = classes[it->type];
    DataPlank *t;
    for ( int i=0; i < plank->kids.length(); i++ ) {
        t = (DataPlank *)plank->kids[i];
        //if ( plank == globals ) qDebug() << "globals delete child";
        if (plank != globals) {
            t->removeSubTree();
            delete t;
        }
    }
    if (c.name != "") {
        foreach ( VariableDefinition v, c.members ) {
            fullName = it->name + "." + v.name;
            //if ( plank == globals ) qDebug() << "globals adding child" << fullName;
            d = addDataPlank(it,it->treeLevel+1,it->map,fullName,
                    v.type);
            if ( it->map == globalMap ) {
                d->address="&(::"+fullName+")";
            } else {
                d->address="&("+fullName+")";
            }
            it->addChild(d);
            dataWindow->request(d);
        }
    } else if (it->userDefined) {
        ArrayBoundsDialog *dialog = new ArrayBoundsDialog();
        if (dialog->exec()) {
            int min = dialog->min;
            int max = dialog->max;
            QString type = it->type;
            for (int i = min; i <= max; i++) {
                fullName = it->name + QString("[%1]").arg(i);
                //d = addDataPlank(it, it->treeLevel,it->map, fullName, type, "");
                //d->address = QString("%1+%2").arg(it->address).arg(it->size * i);
                //it->addChild(d);
                //dataWindow->request(d);
            }
        }
        delete dialog;
    } else if ((n = it->type.indexOf(" *")) > 0) {
        ArrayBoundsDialog *dialog = new ArrayBoundsDialog();
        if (dialog->exec()) {
            int min = dialog->min;
            int max = dialog->max;
            QString type = it->type;
            type.remove(n + 1, 1);
            n = type.length();
            if (type[n - 1] == ' ') type.chop(1);
            for (int i = min; i <= max; i++) {
                fullName = it->name + QString("[%1]").arg(i);
                d = addDataPlank(it,it->treeLevel+1,it->map, fullName, type);
                if (it->address == "$rsp") {
                    d->address = QString("$rsp+%1").arg(i * 8);
                    d->format = "Hexadecimal";
                } else {
                    d->address = "&(" + fullName + ")";
                }
                it->addChild(d);
                dataWindow->request(d);
            }
        }
        delete dialog;
    } else if ((n = it->type.indexOf('[')) >= 0) {
        QString type = it->type;
        int n2 = type.indexOf(']');
        if (n2 < 0) return;
        QString dimString = type.mid(n + 1, n2 - n - 1);
        type.remove(n, n2 - n + 1);
        n = type.length();
        if (type[n - 1] == ' ') type.chop(1);
        ArrayBoundsDialog *dialog = new ArrayBoundsDialog();
        int dim;
        if (dimString.length() > 0) {
            dim = dimString.toInt();
            dialog->setMax(dim - 1);
            dialog->setMin(0);
        }
        if (dialog->exec()) {
            int min = dialog->min;
            int max = dialog->max;
            int n3 = type.indexOf('[');
            int n4 = type.indexOf(']');
            int dim2;
            if (n3 > 0 && n4 > 0) {
                dimString = type.mid(n3 + 1, n4 - n3 - 1);
                dim2 = dimString.toInt() - 1;
                if (dim2 > 9) dim2 = 9;
            } else {
                dim2 = 0;
            }
            int isize = 8;
            QString format;
            if (type.indexOf('[', n4) >= 0) {
                dim2 = 0;
            } else {
                QString newType = type.left(n3 - 1);
                if (simpleTypes.contains(newType)) {
                    isize = sizeForType[newType];
                    if (newType == "char") {
                        format = "Character";
                    } else if (newType == "float" || newType == "double"
                            || newType.indexOf("real") > 0) {
                        format = "Floating point";
                    } else if (newType == "bool" ||
                            newType.indexOf("logical") >= 0) {
                        format = "Boolean";
                    } else {
                        format = "Decimal";
                    }
                }
            }
            for (int i = min; i <= max; i++) {
                fullName = it->name + QString("[%1]").arg(i);
                d = addDataPlank(it,it->treeLevel+1,it->map, fullName, type);
                d->size = isize;
                d->last = dim2;
                d->format = format;
                it->addChild(d);
                dataWindow->request(d);
            }
        }
    } else if (it->isFortran && it->dimensions.size() > 0) {
        ArrayBoundsDialog *dialog = new ArrayBoundsDialog();
        dialog->setMin(it->dimensions[0].first);
        dialog->setMax(it->dimensions[0].last);
        if (dialog->exec()) {
            int min = dialog->min;
            int max = dialog->max;
            QString type = it->type;
            int n1 = type.lastIndexOf("(");
            int n2 = type.indexOf(",");
            if (n2 < 0) {
                type = type.left(n1 - 1);
            } else {
                type = it->type.left(n1) + it->type.mid(n2 + 1);
            }
            int isize = 8;
            QString format;
            if (simpleTypes.contains(type)) {
                isize = sizeForType[type];
                if (type == "char") {
                    format = "Character";
                } else if (type == "float" || type == "double"
                        || type.indexOf("real") > 0) {
                    format = "Floating point";
                } else if (type == "bool" || type.indexOf("logical") >= 0) {
                    format = "Boolean";
                } else {
                    format = "Decimal";
                }
            }

            for (int i = min; i <= max; i++) {
                fullName = it->name + QString("[%1]").arg(i);
                d = addDataPlank(it,it->treeLevel+1,it->map, fullName, type);
                d->size = isize;
                d->format = format;
                it->addChild(d);
                dataWindow->request(d);
            }
        }
    }
}

void DataWindow::request(DataPlank *d)
{
    emit requestVar(d->map, d->name, d->address, d->type, d->format, d->size);
}

void DataWindow::receiveVar(DataMap *map, QString name, QString value)
{
    DataPlank *plank;
    plank = map->value(name);
    if (plank == 0) {
        return;
    }
    plank->setValue(value);
    int n2 = plank->kids.length();
    for (int j = 0; j < n2; j++) {
        request(plank->kids[j]);
    }
}

void DataWindow::receiveVars(DataMap *group, VariableDefinitionMap &vars)
{
    DataPlank *p;
    DataPlank *parent;
    VariableDefinition *v;
    int i=1;
    QStringList keys;

    /*
     *  Start by deactivating any planks which are no longer visible in
     *  the group.
     */
    keys = group->keys();
    //qDebug() << "vars keys" << vars.keys();
    //qDebug() << "group keys" << keys;
    foreach ( QString k, keys ) {
        if ( !vars.contains(k) ) {
            //qDebug() << "vars missing" << k;
            group->value(k)->deactivate();
        }
    }

    /*
     *  Process all the live variables in the group.
     */
    keys = vars.keys();
    //qDebug() << "vars keys" << keys;
    parent = plankForMap[group];
    foreach ( QString k, keys ) {
        //qDebug() << "processing" << k;

        /*
         *      Now either find the plank in the group map or create a new plank.
         */
        p = group->value(k);
        v = &vars[k];
        //qDebug() << "receiveVars" << k << v->type << v->values;
        if ( p == 0 ) {
            p = dataTree->addDataPlank(parent,2, group, v->name,"");
            p->address = QString("&(::%1").arg(v->name);
            p->setName(v->name);
            //parent->addChild(p);
            p->values = new AllTypesArray(v->size);
            i++;
        }

        /*
         *      Update info about the variable
         */
        p->setType(v->type);
        p->size = v->size;
        p->values->resize(p->size);
        p->isFortran = v->isFortran;
        p->dimensions.clear();
        foreach ( Limits d, v->dimensions ) {
            p->dimensions.append(d);
        }

        /*
         *      Reactivate any hidden plank.
         */
        if ( p->hidden ) {
            p->reactivate();
            parent->addChild(p);
        }

        /*
         *      Update the data for the plank.
         */
        p->setValues(v->values);

        /*
         *      Update recursively any kids.
         */
        int n = p->kids.length();
        //qDebug() << "child count" << n;
        for ( int j = 0; j < n; j++ ) {
            request(p->kids[j]);
        }
    }
    parent->sortChildren();
}

void DataWindow::receiveGlobals(VariableDefinitionMap vars)
{
    //qDebug() << "receiveGlobals";
    receiveVars ( globalMap, vars );
}

void DataWindow::receiveLocals(VariableDefinitionMap vars)
{
    //qDebug() << "receiveLocals";
    receiveVars ( localMap, vars );
}

void DataWindow::receiveParameters(VariableDefinitionMap vars)
{
    //qDebug() << "receiveParameters";
    receiveVars ( parameterMap, vars );
}

void DataPlank::removeChild(DataPlank *p)
{
    int i;
    //qDebug() << "removeChild" << this << name << p << p->name;
    for ( i = 0; i < kids.length(); i++ ) {
        //qDebug() << "kid" << i << kids[i] << kids[i]->name;
        if ( kids[i] == p ) {
            //qDebug() << "found" << p << i;
            kids.remove(i);
            break;
        }
    }
}

void DataPlank::deactivate()
{
    //qDebug() << "deactivate" << this->name;
    hide();
    hidden = true;
    parent->removeChild(this);
}

void DataPlank::reactivate()
{
    //qDebug() << "reactivate" << this->name;
    show();
    hidden = false;
}

void DataPlank::removeSubTree()
{
    int i;
    int n = kids.length();
    DataPlank *plank;

    for (i = 0; i < n; i++) {
        plank = kids[i];
        plank->removeSubTree();
    }
    for (i = 0; i < n; i++) {
        plank = (DataPlank *)kids[i];
        plank->map->remove(plank->name);
        delete plank;
    }
}

DataPlank *DataTree::addDataPlank(DataPlank *parent, int theLevel, DataMap *map,
        QString n, QString t)
{

    //qDebug() << "addDataPlank" << i << theLevel << n << t << v << planks;
    DataPlank *d = new DataPlank(this);     
    d->treeLevel = theLevel;
    d->show();
    d->hidden = false;
    planks++;
    //table.append(d);
    setCurrentPlank(d);
    d->tree = this;
    d->parent = parent;
    if ( parent ) parent->kids.append(d);
    d->setName(n);
    d->setType(t);
    //d->setValue(v);
    d->format = "string";
    d->size = 0;
    d->userDefined = (map == userDefinedMap);
    if (map == globalMap) {
        d->address = QString("&(::%1)").arg(n);
    } else {
        d->address = QString("&(%1)").arg(n);
    }
    d->map = map;
    map->insert(n, d);
    return d;
}

void DataTree::redisplay ( DataPlank *p, EZ::Color highlight )
{
    QString s;
    QString t;
    QStringList parts;
    setCurrentPlank(p);
    int size = p->size;
    int left = size;
    int max=0;
    int k = 0;
    int count;
    QString format = p->format;
    int num;
    int span;

    FormatFunction f=formatToFunction[format];
    setCurrentPlank(p);
    if ( format == "string" || format == "string array" ) {
        span = columns;
        num = p->stringValues.length();
        if ( num < 1 ) num = 1;
        rows = num;
        left = rows;
        size = 1;
        count = 1;
    } else {
        span = formatToSpan[format];
        count = columns/span;
        num = size/formatToSize[format];
        if ( num < 1 ) num = 1;
        size = formatToSize[format];
        rows = (num + count - 1) / count;
    }

    setRowCount(rows);
    for ( int i=0; i < p->treeLevel - 1; i++ ) {
        setSpan(0,i,1,1);
        setText(0,i,"*");
    }
    setSpan(0,p->treeLevel,1,1);
    setText(0,p->treeLevel," ");
    setSpan(0,p->treeLevel+1,1,maxLevels-p->treeLevel-1);
    setText(0,p->treeLevel+1,p->name);
    setSpan(0,maxLevels+1,1,1);
    setText(0,maxLevels+1,p->type);


    for ( int r = 0; r < rows; r++ ) {
        for ( int c = 0; c < columns; c++ ) {
            setSpan(r,c+maxLevels+2,1,1);
        }
        if ( r > 0 ) {
            setSpan(r,0,1,maxLevels+1);
            setText(r,0," ");
            setSpan(r,maxLevels+1,1,1);
            setText(r,maxLevels+1," ");
        }
        max = count;
        if ( max > left/size ) max = left/size;
        for ( int j=0; j < max; j++ ) {
            s = (*f)(p->values,p->stringValues,k,j);
            setSpan(r,j*span+maxLevels+2,1,span);
            setText(r,j*span+maxLevels+2,s,highlight);
            k++;
        }
        for ( int j = max; j < count; j++ ) {
            setSpan(r,j*span+maxLevels+2,1,span);
            setText(r,j*span+maxLevels+2,"");
        }
        left -= max/size;
    }
    //if ( v == planks - 1 ) {
    //resizeToFitContents(maxLevels);
    //}
}
