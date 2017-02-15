#include "datawindow.h"
#include "backtracewindow.h"
#include "gdb.h"
#include <cstdio>
#include <QMenu>
#include <QContextMenuEvent>
#include <QScrollBar>

static int maxLevels = 12;

extern GDB *gdb;
extern BackTraceWindow *backTraceWindow;
extern IntHash sizeForType;
extern QSet<QString> simpleTypes;
extern DataWindow *dataWindow;
extern IntHash formatToSpan;
extern IntHash formatToSize;
extern QHash<QString,FormatFunction> formatToFunction;

extern EZPlank *latestPlank;
extern bool running;

QColor bgColor;
QColor expandColor;
QColor collapseColor;

DataTree *dataTree;
DataPlank *globals;
DataMap globalMap;
DataMap functionMap;
DataMap expansionMap;
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

    scrollArea = new QScrollArea;
    dataTree = new DataTree(this);
    //dataTree->setShowGrid(false);
    scrollArea->setWidget(dataTree);
    layout->addWidget(scrollArea);
    dataTree->setToolTip(tr("Left click on a > symbol to expand a variable.\n"
                "Right click to change formats."));

    setLayout(layout);

    qRegisterMetaType<DataPlank*>("DataPlank*");
    //qRegisterMetaType<DataMap*>("DataMap*");
    //qRegisterMetaType<DataMap> ("DataMap");
    qRegisterMetaType<VariableDefinitionMap> ("VariableDefinitionMap");

    connect(dataTree, SIGNAL(requestParameters(DataPlank*,int)),
            gdb, SLOT(requestParameters(DataPlank*,int)));
    connect(dataTree, SIGNAL(requestLocals(DataPlank*,int)),
            gdb, SLOT(requestLocals(DataPlank*,int)));
    connect(gdb, SIGNAL(sendBackTrace(QStringList)), dataTree,
            SLOT(receiveBackTrace(QStringList)));
    connect(gdb, SIGNAL(sendGlobals(VariableDefinitionMap)), this,
            SLOT(receiveGlobals(VariableDefinitionMap)));
    connect(gdb, SIGNAL(sendLocals(DataPlank*,VariableDefinitionMap)), this,
            SLOT(receiveLocals(DataPlank*,VariableDefinitionMap)));
    connect(gdb, SIGNAL(sendParameters(DataPlank*,VariableDefinitionMap)), this,
            SLOT(receiveParameters(DataPlank*,VariableDefinitionMap)));
    connect ( this, SIGNAL(requestTypedef(QString,QString&)),
              gdb, SLOT(processTypedefRequest(QString,QString&)),
              Qt::BlockingQueuedConnection );
    connect ( this, SIGNAL(requestClass(QString,ClassDefinition&)),
              gdb, SLOT(processClassRequest(QString,ClassDefinition&)),
              Qt::BlockingQueuedConnection );
    connect ( this,
        SIGNAL(requestVar(DataPlank*,QString,QString,QString,QString,int,int)),
        gdb,
        SLOT(requestVar(DataPlank*,QString,QString,QString,QString,int,int)) );
    connect ( gdb, SIGNAL(sendVar(DataPlank*,QString,QStringList)),
            this, SLOT(receiveVar(DataPlank*,QString,QStringList)) );
    //connect ( gdb, SIGNAL(dataReady(QStringList)),
    //this, SLOT(setData(QStringList)) );
    connect(dataTree, SIGNAL(requestReset()), gdb, SLOT(requestReset()));
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
    //DataPlank *plank;
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
    //plank = userDefinedMap->value(name);
    //if (plank == 0) {
        //plank = dataTree->addDataPlank(userDefined,userDefinedMap, name, strings[2]);
    //}
    //plank->address = strings[1];
    //if (varToAddress.contains(name)) {
        //plank->address = varToAddress[name];
    //}
    //if (plank->address == "") {
        //plank->address = QString("&(%1)").arg(name);
    //}
    //if (strings[3] == "string" && strings[5] == "0") strings[5] = "1";
    ////plank->setRange(strings[4].toInt(), strings[5].toInt());
    //plank->setType(strings[2]);
    //plank->format = strings[3];
    ////userDefined->addChild(plank);
    ////request(plank);
}

void DataWindow::resetData()
{
    QString s;
    //qDebug() << "resetData";
    saveScroll();
    dataTree->finalPlank(dataTree->all)->isFinal = true;
    dataTree->buildTree(dataTree->all);
    dataTree->table.clear();
    dataTree->reorder(dataTree->all);
    //dataTree->resizeToFitContents(maxLevels);
    restoreScroll();
}

void DataWindow::rebuildData()
{
    QString s;
    saveScroll();
    dataTree->table.clear();
    dataTree->reorder(dataTree->all);
    dataTree->resizeToFitContents(maxLevels);
    restoreScroll();
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
    format = "";
    isSimple = true;
    //isFortran = false;
    isFinal = false;
    indicator = 0;
    needsRequest = true;
    frame = 0;
    state = EZ::Unknown;
    //setFlags(Qt::ItemIsSelectree | Qt::ItemIsEnabled);
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
    fullName = n;
    //qDebug() << "shortName" << shortName;
    if ( rows < 1 ) tree->setRowCount(1);
    if ( treeLevel > 0 ) {                       // Assume treeLevel is 1
        int i;
        for ( i=0; i < treeLevel-1; i++ ) {
            tree->setSpan(0,i,1,1);              // Empty space before indicator
            tree->setText(0,i," ",EZ::Default);  // size treeLevel - 1
        }
        tree->setSpan ( 0, i, 1, 1 );            // Column 0 spans 1 column
        tree->setText(0,i," ",EZ::Default);      // Indicator in 0
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
    ClassDefinition c;
    QString newType;
    if ( t == type ) return;

    //qDebug() << "new type for" << name;
    type = t;
    int n=t.indexOf("[");
    if ( n < 0 ) n = t.indexOf("(");
    if ( n < 0 ) n = t.indexOf("*");
    if ( n >= 0 ) {
        basicType = t.left(n).trimmed();
    } else {
        basicType = t;
    }
    //qDebug() << "setType" << name << type << basicType;
    //foreach ( Limits limits, dimensions ) {
        //qDebug() << "limits" <<  limits.first << limits.last;
    //}

    if ( !classes.contains(basicType) && !simpleTypes.contains(basicType) ) {
        //qDebug() << "typedef ?"  << basicType;
        if ( dataWindow != 0 ) dataWindow->getTypedef(basicType,newType);
        //qDebug() << "after getTypedef" << newType;
        if ( newType != "" ) {
            type.replace(basicType,newType);
            basicType = newType;
        }
        if ( !classes.contains(basicType) && !simpleTypes.contains(basicType) ) {
            if ( dataWindow != 0 ) dataWindow->getClass(basicType,c);
            classes[basicType] = c;
        }
    }

    //qDebug() << basicType << format;
    if (sizeForType.contains(t)) {
        size = sizeForType[t];
    } else {
        size = 8;
    }

    isSimple = simpleTypes.contains(t);
    //qDebug() << basicType << format << size << isSimple;
    if (isSimple) {
        basicType = type;
        state = EZ::Simple;
        if ( formatForType.contains(t) ) {
            format = formatForType[t];
        } else {
            format = QString("hex%1").arg(size);
        }
    } else if (t == "char **") {
        state = EZ::Expanded;
        format = "string array";
    } else if ( t == "char *" ) {
        format = "string";
        state = EZ::Simple;
    } else if ( t == "std::string" ) {
        format = "std::string";
        state = EZ::Simple;
    } else if (name == "stack" && parent == tree->globals ) {
        format = QString("hex%1").arg(size);
        state = EZ::Expanded;
    } else if (t.indexOf(" *") >= 0) {
        format = QString("hex%1").arg(size);
        //qDebug() << "setType" << name << t << "collapse";
        if ( state == EZ::Unknown ) state = EZ::Collapsed;
    } else if ( dimensions.size() >= 2 ) {
        if ( state == EZ::Unknown ) {
            if ( formatForType.contains(basicType) ) {
                format = formatForType[basicType];
            } else {
                format = QString("hex%1").arg(size);
            }
            state = EZ::Collapsed;
        }
    } else if ( formatForType.contains(t) ) {
        format = formatForType[t];
        state = EZ::Simple;
        //qDebug() << "formatForType contains type" << name << t;
    } else if ( formatForType.contains(basicType) ) {
        format = formatForType[basicType];
        state = EZ::Simple;
        //qDebug() << "formatForType contains basic type" << name << t << basicType;
    } else if ( sizeForType.contains(basicType) ) {
        size = sizeForType[basicType];
        if ( size == 0 ) size = 8;
        format = QString("hex%1").arg(size);
        state = EZ::Simple;
        //qDebug() << "sizeForType contains basic type" << name << t << basicType;
    } else {
        format = "hex1";
        //qDebug() << "setType" << name << t << "collapse";
        if ( state == EZ::Unknown ) state = EZ::Collapsed;
        //qDebug() << "else ?" << name << t << basicType;
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
    bool fault = false;
    int k = 0;

    if ( !running ) return;

    if ( format == "string" || format == "std::string" ||
         format == "string array" )  {
        stringValues = s;
    } else {
        int n = (size + 7)/8;
        if ( n > 100000 ) {
            qDebug() << "datawindow setValues" << size << n;
            return;
        }
        for ( int j = 0; j < n; j++ ) values->u8(j) = 0;
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
            int n = parts.length();
            if ( n > size-k ) n = size-k;
            for ( int r=0; r < n; r++ ) {
                values->u1(k) = parts[r].toInt(&ok,16);
                if ( !ok || fault ) {
                    fault = true;
                    values->u1(k) = 0;
                }
                k++;
            }
        }
    }
    if ( !fault ) size = k;
}

void DataTree::setBinary()
{
    DataPlank *d = (DataPlank *)latestPlank;
    d->format = QString("bin%1").arg(sizeForType[d->basicType]);
    dataWindow->resetData();
}

void DataTree::setBool()
{
    DataPlank *d = (DataPlank *)latestPlank;
    d->format = QString("bool%1").arg(sizeForType[d->basicType]);
    dataWindow->resetData();
}

void DataTree::setDecimal()
{
    DataPlank *d = (DataPlank *)latestPlank;
    d->format = QString("dec%1").arg(sizeForType[d->basicType]);
    dataWindow->resetData();
}

void DataTree::setUnsignedDecimal()
{
    DataPlank *d = (DataPlank *)latestPlank;
    d->format = QString("udec%1").arg(sizeForType[d->basicType]);
    dataWindow->resetData();
}

void DataTree::setHexadecimal()
{
    DataPlank *d = (DataPlank *)latestPlank;
    d->format = QString("hex%1").arg(sizeForType[d->basicType]);
    dataWindow->resetData();
}

void DataTree::setCharacter()
{
    DataPlank *d = (DataPlank *)latestPlank;
    d->format = "character";
    dataWindow->resetData();
}

void DataTree::setFloatingPoint()
{
    DataPlank *d = (DataPlank *)latestPlank;
    d->format = sizeForType[d->basicType] == 4 ? "float" : "double";
    dataWindow->resetData();
}

void DataTree::setBinaryFP()
{
    DataPlank *d = (DataPlank *)latestPlank;
    d->format = QString("binaryfp%1").arg(sizeForType[d->basicType]);
    dataWindow->resetData();
}

void DataTree::setFields()
{
    DataPlank *d = (DataPlank *)latestPlank;
    d->format = QString("fields%1").arg(sizeForType[d->basicType]);
    dataWindow->resetData();
}

void DataTree::contextMenuEvent(QContextMenuEvent * /*event*/)
{
    DataPlank *p= (DataPlank *)latestPlank;
    QString type = p->type;
    //qDebug() << p->name << type << p->value();
    if (type.indexOf("char") >= 0) {
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
    //for ( int p=0; p < planks; p++ ) {
        //if ( table[p] ) {
            //table[p]->hide();
            //delete table[p];
        //}
    //}

    EZTable::setPlankCount(n);
    //table.resize(n);
    //DataPlank *plank;
    //for ( int p=0; p < n; p++ ) {
        //table[p] = plank = 
        //plank->show();
    //}
    //planks = n;
}


DataTree::DataTree(QWidget *parent)
: EZTable(parent)
{
    //DataPlank *stack;
    columns = ebe["data/columns"].toInt();
    setPlankCount(0);
    setColumnCount(columns+maxLevels+2);

    bgColor = QColor(ebe["table_bg"].toString());
    expandColor = QColor("#00c000");
    collapseColor = QColor("#c00000");
    //horizontalHeader()->show();

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
    all = addDataPlank(0,0,0, tr("Name"), tr("Type"));
    all->needsRequest = false;
    all->format = "string";
    all->state = EZ::Simple;
    setSpan(0,maxLevels+2,1,columns);
    setText(0,maxLevels+2,tr("Value"));
    all->stringValues.append(tr("Value"));
    all->state = EZ::Expanded;

    ::globals = globals = addDataPlank(all,0,0, tr("globals"), "");
    globals->needsRequest = false;
    plankForMap[&globalMap] = globals;

    globals->state = EZ::Expanded;
    globals->format = "string";
    globals->parent = 0;

    for ( int c=0; c < columns+2; c++ ) {
        setColumnWidth(c, fontWidth*0.8);
    }

    buildTree(all);
}

void DataTree::reorder(DataPlank *p)
{
    table.append(p);
    if ( p->state == EZ::Expanded ) {
        for ( int j=0; j < p->kids.size(); j++ ) {
            reorder(p->kids[j]);
        }
    }
}

void DataTree::buildTree(DataPlank *p)
{
    qDebug() << "buildTree" << p << p->name << p->treeLevel
             << p->format << p->state << p->kids.size();
    if ( p->needsRequest ) dataWindow->request(p);
    else redisplay(p, EZ::Highlight);
    if ( p->state == EZ::Expanded ) {
        for ( int j=0; j < p->kids.size(); j++ ) {
            buildTree(p->kids[j]);
        }
    }
    qDebug() << "end BT";
}

DataPlank * DataTree::finalPlank(DataPlank *p)
{
    DataPlank *final = 0;
    if ( p->needsRequest ) final = p;
    DataPlank *f;
    p->isFinal = false;
    if ( p->state == EZ::Expanded ) {
        for ( int j=0; j < p->kids.size(); j++ ) {
            f = finalPlank(p->kids[j]);
            if ( f ) final = f;
        }
    }
    return final;
}

DataTree::~DataTree()
{
    globals->removeSubTree();
    delete globals;
}

void DataTree::collapseDataPlank(DataPlank *p)
{
    p->state = EZ::Collapsed;
    //qDebug() << "collapseDataPlank" << p->name;
    foreach ( DataPlank *k, p->kids ) {
        k->deactivate();
    }
    foreach ( DataPlank *k, p->kids ) {
        p->removeChild(k);
    }
    redisplay(p,EZ::NoChange);
    dataWindow->rebuildData();
}

void DataTree::expandDataPlank(DataPlank *p)
{
    DataPlank *d;
    ClassDefinition c;
    int n;

    c = classes[p->type];

    //qDebug() << "expand" << p->name << p->type << c.name;
    if (c.name != "") {
        p->state = EZ::Expanded;
        n = c.members.size();
        VariableDefinition v;
        for ( int i = 0; i < n; i++ ) {
            v = c.members[i];
            d = addDataPlank(p,p->treeLevel+1,&expansionMap,v.name,v.type);
            d->fullName = p->fullName + "." + v.name;
            //qDebug() << "adding class child" << d->fullName << v.type;
            d->frame = p->frame;
            d->values = new AllTypesArray(d->size);
            redisplay(d,EZ::Highlight);
            if ( p->map == &globalMap ) {
                d->address="&(::"+d->fullName+")";
            } else {
                d->address="&("+d->fullName+")";
            }
            d->isFinal = i == n-1;
            //dataWindow->request(d);
        }
        dataWindow->resetData();
        //qDebug() << "Done with expand";
    } else if ( p->format == "string" || p->format == "string array" ) {
        p->state = EZ::Expanded;
        foreach ( DataPlank *k, p->kids ) {
            k->reactivate();
        }
        redisplay(p,EZ::NoChange);
        dataWindow->resetData();
    } else if ((n = p->type.indexOf(" *")) > 0) {
        p->state = EZ::Expanded;
        //qDebug() << "expand pointer";
        ArrayBoundsDialog *dialog = new ArrayBoundsDialog();
        if (dialog->exec()) {
            if ( dialog->arrayCheck->isChecked() ) {
                QString type = p->type;
                type.remove(n + 1, 1);
                n = type.length();
                if (type[n - 1] == ' ') type.chop(1);
                //qDebug() << "type now" << type;
                d = addDataPlank(p,p->treeLevel+1,&expansionMap, "*"+p->name, type);
                d->size = sizeForType[type] * dialog->n;
                d->values = new AllTypesArray(d->size);
                d->frame = p->frame;
                if (p->address == "$rsp") {
                    d->address = "$rsp";
                    d->format = "hexadecimal";
                } else {
                    d->address = p->name;
                    d->format = formatForType[type];
                }
                //p->addChild(d);
                d->isFinal = true;
                dataWindow->request(d);
                //dataWindow->resetData();
            } else {
                p->state = EZ::Expanded;
                QString type = p->type;
                type.remove(n + 1, 1);
                n = type.length();
                if (type[n - 1] == ' ') type.chop(1);
                //qDebug() << "type now" << type;
                c = classes[type];
                n = c.members.size();
                VariableDefinition v;
                p->isFinal = false;
                //dataWindow->request(p);
                for ( int i = 0; i < n; i++ ) {
                    v = c.members[i];
                    d = addDataPlank(p,p->treeLevel+1,&expansionMap,v.name,v.type);
                    d->fullName = p->fullName + "->" + v.name;
                    //qDebug() << "adding class child" << d->fullName << v.type;
                    d->frame = p->frame;
                    d->values = new AllTypesArray(d->size);
                    redisplay(d,EZ::Highlight);
                    if ( p->map == &globalMap ) {
                        d->address="&(::"+d->fullName+")";
                    } else {
                        d->address="&("+d->fullName+")";
                    }
                    d->isFinal = i == n-1;
                    //qDebug() << "request" << d->fullName << d->address << d->isFinal;
                    dataWindow->request(d);
                }
                //dataWindow->resetData();
                //qDebug() << "Done with expand";
            }
        }
        delete dialog;
    } else if ( p->dimensions.size() >= 2 ) {
        QString kidName;
        int n1 = p->type.indexOf("[");
        int n2;
        bool isC = n1 >= 0;
        QString kidType;
        if ( isC ) {
            n2 = p->type.indexOf("]");
        } else {
            n1 = p->type.indexOf("(");
            n2 = p->type.indexOf(")");
        }
        kidType = p->type.left(n1) + p->type.mid(n2+1);
        for ( int i=p->dimensions[0].first; i <= p->dimensions[0].last; i++ ) {
            if ( isC ) {
                kidName = p->name+QString("[%1]").arg(i);
            } else {
                kidName = p->name+QString("(%1)").arg(i);
            }
            d = addDataPlank(p,p->treeLevel+1,&expansionMap,kidName,kidType);
            for ( int j=1; j < p->dimensions.size(); j++ ) {
                d->dimensions.append(p->dimensions[j]);
            }
            d->frame = p->frame;
            d->address = kidName;
            d->fullName = kidName;
            d->format = p->format;
            if ( d->dimensions.size() == 1 ) {
                d->size = d->dimensions[0].last - d->dimensions[0].first + 1;
                if ( sizeForType.contains(d->basicType) ) {
                    d->size *= sizeForType[d->basicType];
                } else {
                    d->size *= 8;
                }
            } else {
                d->size = 8;
            }
            d->values = new AllTypesArray(d->size);
        }
        dataWindow->resetData();
    } else {
        //qDebug() << "expanding type" << p->type;
        p->state = EZ::Expanded;
        foreach ( DataPlank *k, p->kids ) {
            k->reactivate();
        }
        dataWindow->resetData();
    }
}

void DataWindow::getTypedef(QString type, QString &newType)
{
    emit requestTypedef(type, newType);
}

void DataWindow::getClass(QString type, ClassDefinition &c)
{
    emit requestClass(type, c);
}

void DataWindow::request(DataPlank *d)
{
    //qDebug() << "request" << d->name << d->address << d->format <<
        //d->size << d->frame;
    emit requestVar(d, d->name, d->address, d->type, d->format, d->size,
                    d->frame);
}

void DataWindow::receiveVar(DataPlank *p, QString /*name*/, QStringList values)
{
    //qDebug() << "receiveVar" << p->name << p << values << p->isFinal;
    p->setValues(values);
    dataTree->redisplay(p,EZ::Highlight);
    //dataWindow->saveScroll();
    //dataTree->table.clear();
    //dataTree->reorder(dataTree->all);
    //dataTree->resizeToFitContents(maxLevels);
    //dataWindow->restoreScroll();
    //int n2 = plank->kids.size();
    //for (int j = 0; j < n2; j++) {
        //request(plank->kids[j]);
    //}
    if ( p->isFinal ) dataWindow->rebuildData();
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
    //qDebug() << "group keys" << keys << group;
    parent = plankForMap[group];
    //qDebug() << "parent" << parent;
    foreach ( QString k, keys ) {
        if ( !vars.contains(k) ) {
            //qDebug() << "vars missing" << k;
            group->value(k)->deactivate();
            parent->removeChild(group->value(k));
            group->remove(k);
        }
    }

    /*
     *  Process all the live variables in the group.
     */
    keys = vars.keys();
    //qDebug() << "vars keys" << keys;
    foreach ( QString k, keys ) {
        //qDebug() << "processing" << k;

        /*
         *      Now either find the plank in the group map or create a new plank.
         */
        p = group->value(k);
        //qDebug() << "p" << p;
        v = &vars[k];
        //qDebug() << k << parent->frame << v->values;
        //qDebug() << "receiveVars" << k << v->type << v->size << v->values;
        if ( p == 0 ) {
            p = dataTree->addDataPlank(parent,parent->treeLevel+1, group, v->name,"");
            if ( group == &globalMap ) {
                p->frame = 0;
                if ( v->name == "stack" ) {
                    p->address = QString("$rsp");
                    p->setName(v->name);
                    p->size = 64;
                    p->values = new AllTypesArray(v->size);
                } else {
                    p->address = QString("&(::%1)").arg(v->name);
                    p->setName(v->name);
                    p->values = new AllTypesArray(v->size);
                    p->size = v->size;
                }
            } else {
                if ( v->type == "std::string" ) {
                    p->address = v->name;
                } else {
                    p->address = QString("&(%1)").arg(v->name);
                }
                p->setName(v->name);
                p->values = new AllTypesArray(v->size);
                p->size = v->size;
            }
            p->setValues(v->values);
            foreach ( Limits limits, v->dimensions ) {
                p->dimensions.append(limits);
            }
            dataTree->redisplay(p,EZ::NoChange);
            i++;
        }

        //qDebug() << "var" << p->name << k << p;
        /*
         *      Update info about the variable
         */
        p->setType(v->type);
        p->size = v->size;
        //qDebug() << v->name << v->size;
        p->values->resize(p->size);
        //p->isFortran = v->isFortran;
        p->dimensions.clear();
        foreach ( Limits d, v->dimensions ) {
            p->dimensions.append(d);
        }

        /*
         *      Reactivate any hidden plank.
         */
        if ( p->hidden && parent->state == EZ::Expanded ) {
            //qDebug() << "reactivate" << p;
            p->reactivate();
            parent->addChild(p);
        }

        /*
         *      Update the data for the plank.
         */
        //qDebug() << v->name << "about to set values";
        //p->setValues(v->values);
        //dataTree->redisplay(p,EZ::NoChange);

        /*
         *      Update recursively this and any kid's frame.
         */
        p->updateFrame(parent->frame);
    }
}

void DataWindow::receiveGlobals(VariableDefinitionMap vars)
{
    //qDebug() << "receiveGlobals";
    receiveVars ( &globalMap, vars );
}

void DataWindow::receiveLocals(DataPlank *p, VariableDefinitionMap vars)
{
    //qDebug() << "receiveLocals" << p << p->isFinal;
    receiveVars ( &(p->localMap), vars );
    //if ( p->isFinal ) dataTree->resizeToFitContents(maxLevels);
}

void DataWindow::receiveParameters(DataPlank *p, VariableDefinitionMap vars)
{
    //qDebug() << "receiveParameters" << p;
    receiveVars ( &(p->parameterMap), vars );
}

void DataPlank::removeChild(DataPlank *p)
{
    int i;
    //qDebug() << "removeChild" << this << name << p << p->name;
    for ( i = 0; i < kids.size(); i++ ) {
        //qDebug() << "kid" << i << kids[i] << kids[i]->name;
        if ( kids[i] == p ) {
            //qDebug() << "found" << p << i;
            kids.remove(i);
            break;
        }
    }
}

void DataPlank::addChild(DataPlank *p)
{
    int i;
    for ( i = 0; i < kids.size(); i++ ) {
        if ( kids[i] == p ) return;
    }
    kids.append(p);
}

void DataPlank::updateFrame(int f)
{
    frame = f;
    if ( state == EZ::Expanded ) {
        for ( int i=0; i < kids.size(); i++ ) {
            kids[i]->updateFrame(f);
        }
    }
}

void DataPlank::deactivate()
{
    //qDebug() << "deactivate" << this->name;
    hide();
    hidden = true;
    if ( state == EZ::Expanded ) {
        for ( int i=0; i < kids.size(); i++ ) {
            kids[i]->deactivate();
        }
    }
    //if ( parent ) parent->removeChild(this);
}

void DataPlank::reactivate()
{
    //qDebug() << "reactivate" << this->name;
    show();
    hidden = false;
    if ( state == EZ::Expanded ) {
        for ( int i=0; i < kids.size(); i++ ) {
            kids[i]->reactivate();
        }
    }
}

void DataPlank::removeSubTree()
{
    int i;
    int n = kids.size();
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

    //qDebug() << "addDataPlank" << theLevel << n << t;
    DataPlank *d = new DataPlank(this);     
    d->treeLevel = theLevel;
    d->show();
    d->hidden = false;
    planks++;
    //table.append(d);
    setCurrentPlank(d);
    setRowCount(1);
    setColumnCount(maxLevels+2+columns);
    if ( theLevel > 0 ) {
        d->indicator = new IndicatorButton((QWidget *)d);
        addWidget(d->indicator,0,theLevel);
    }
    d->tree = this;
    d->parent = parent;
    if ( parent ) parent->addChild(d);
    d->setName(n);
    d->setType(t);
    //d->userDefined = (map == userDefinedMap);
    if (map == &globalMap) {
        d->address = QString("&(::%1)").arg(n);
    } else {
        d->address = QString("&(%1)").arg(n);
    }
    if ( map != 0 ) {
        d->map = map;
        map->insert(n, d);
    }
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
    //qDebug() << p << p->name << p->format;
    if ( format == "string" || format == "std::string" ||
         format == "string array" ) {
        span = columns;
        num = p->stringValues.length();
        if ( num < 1 ) num = 1;
        rows = num;
        left = rows;
        size = 1;
        count = 1;
    } else {
        span = formatToSpan[format];
        if ( span == 0 ) {
            count = 1;
        } else {
            count = columns/span;
        }
        if ( formatToSize.contains(format) ) {
            num = size/formatToSize[format];
            size = formatToSize[format];
        } else {
            num = 0;
            size = 0;
        }
        if ( num < 1 ) num = 1;
        rows = (num + count - 1) / count;
    }
    //qDebug() << "redisplay" << p->name << p->format << rows;
    //qDebug() << "redisplay" << p->name << p->size << rows << left << size << count << num << format;
    if ( p->state == EZ::Collapsed ) rows = 1;
    //if ( rows > 1 ) p->state = EZ::Expanded;
    setRowCount(rows);
    for ( int i=0; i < p->treeLevel - 1; i++ ) {
        setSpan(0,i,1,1);
        setText(0,i,"  ");
    }
    setSpan(0,p->treeLevel,1,1);
    setText(0,p->treeLevel," ");
    setSpan(0,p->treeLevel+1,1,maxLevels-p->treeLevel-1);
    setText(0,p->treeLevel+1,p->name);
    setSpan(0,maxLevels+1,1,1);
    setText(0,maxLevels+1,p->type);
    if ( num < 1 || size < 1 ) return;

    for ( int r = 0; r < rows; r++ ) {
        //for ( int c = 0; c < columns; c++ ) {
            //setSpan(r,c+maxLevels+2,1,1);
        //}
        if ( r > 0 ) {
            setSpan(r,0,1,1);
            setText(r,0,"");
            setSpan(r,1,1,maxLevels);
            setText(r,1,"");
            setSpan(r,maxLevels+1,1,1);
            setText(r,maxLevels+1,"");
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
        left -= max*size;
    }
}

void DataTree::receiveBackTrace(QStringList s)
{
    int frame;
    QStringList parts;
    QString name;
    IntHash highest;
    DataPlank *p;
    DataPlank *kid;
    int n = s.length();
    if ( !running ) return;
    if ( s[n-1][0] != QChar('#') ) n--;
    for ( int i=0; i < n; i++ ) {
        if ( s[i][0] != QChar('#') ) continue;
        //qDebug() << s[i];
        parts = s[i].split(QRegExp("\\s+"));
        //qDebug() << parts;
        if ( parts.length() < 4 ) return;
        if ( parts[2] == "in" ) {
            name = parts[3];
        } else {
            name = parts[1];
        }
        if ( highest.contains(name) ) {
            highest[name]++;
            name = QString("%1:%2").arg(name).arg(highest[name]);
        } else {
            highest[name] = 0;
        }
        //qDebug() << "name" << name;
        if ( !functionMap.contains(name) ) {
            p = addDataPlank(all,0,0, name, "");
            p->needsRequest = false;
            p->format = "string";
            p->state = EZ::Expanded;
            functionMap[name] = p;
            kid = addDataPlank(p,1,0, "parameters", "");
            kid->needsRequest = false;
            kid->format = "string";
            plankForMap[&(kid->parameterMap)] = kid;
            //qDebug() << "plank" << kid << "par map" << &(kid->parameterMap);
            kid->state = EZ::Expanded;
            kid = addDataPlank(p,1,0, "locals", "");
            kid->needsRequest = false;
            kid->format = "string";
            plankForMap[&(kid->localMap)] = kid;
            //qDebug() << "plank" << kid << "localmap" << &(kid->localMap);
            kid->state = EZ::Expanded;
        }
    }
    int m = all->kids.size();
    for ( int i = m-1; i >= 0; i-- ) {
        kid = all->kids[i];
        kid->deactivate();
    }
    all->kids.clear();
    all->kids.append(globals);
    globals->reactivate();
    int k;
    for ( int i=0; i < n; i++ ) {
        if ( s[i][0] != QChar('#') ) continue;
        parts = s[i].split(QRegExp("\\s+"));
        frame = parts[0].mid(1).toInt();
        //qDebug() << parts[0].mid(1) << frame;
        if ( parts[2] == "in" ) {
            name = parts[3];
        } else {
            name = parts[1];
        }
        k = highest[name];
        if ( k != 0 ) {
            highest[name] = k-1;
            name = QString("%1:%2").arg(name).arg(k);
        }
        p = functionMap[name];
        //qDebug() << "function" << name << p << p->kids[0] << p->kids[1];
        p->reactivate();
        all->kids.append(p);
        p->frame = frame;
        p->kids[0]->frame = frame;
        p->kids[0]->isFinal = false;
        //p->kids[0]->kids.clear();
        p->kids[1]->frame = frame;
        p->kids[1]->isFinal = (i == n-1);
        //qDebug() << "rbt" << p->kids[1] << p->kids[1]->isFinal;
        //p->kids[1]->kids.clear();
        emit requestParameters ( p->kids[0], frame );
        emit requestLocals ( p->kids[1], frame );
    }
    emit requestReset();
}

IndicatorButton::IndicatorButton(QWidget *parent)
    : QPushButton(parent)
{
    state = &((DataPlank *)parent)->state;
    //qDebug() << "IndicatorButton" << state;
}

void IndicatorButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    int h = height();
    int w = width();
    painter.fillRect(0,0,w,h,bgColor);
    if ( *state == EZ::Collapsed ) {
        {
            QPoint pts[4] = {
                QPoint(w*.4,h*.55),
                QPoint(w*.9,h*.55),
                QPoint(w*.9,h*.45),
                QPoint(w*.4,h*.45),
            };
            //qDebug() << "pts" << pts[0] << pts[1] << pts[2];
            painter.setPen(QPen(expandColor));
            painter.setBrush(QBrush(expandColor));
            painter.drawConvexPolygon(pts,4);
        }
        {
            QPoint pts[4] = {
                QPoint(w*.7,h*.75),
                QPoint(w*.7,h*.25),
                QPoint(w*.6,h*.25),
                QPoint(w*.6,h*.75),
            };
            //qDebug() << "pts" << pts[0] << pts[1] << pts[2];
            painter.setPen(QPen(expandColor));
            painter.setBrush(QBrush(expandColor));
            painter.drawConvexPolygon(pts,4);
        }
    } else if ( *state == EZ::Expanded ) {
        QPoint pts[4] = {
            QPoint(w*.4,h*.55),
            QPoint(w*.9,h*.55),
            QPoint(w*.9,h*.45),
            QPoint(w*.4,h*.45),
        };
        //qDebug() << "pts" << pts[0] << pts[1] << pts[2];
        painter.setPen(QPen(collapseColor));
        painter.setBrush(QBrush(collapseColor));
        painter.drawConvexPolygon(pts,4);
    }
}

void IndicatorButton::mouseReleaseEvent(QMouseEvent *)
{
    DataPlank *p = (DataPlank *)parent();
    DataTree *tree = p->tree;
    //qDebug() << "plank" << p << "    tree" << tree;

    if ( *state == EZ::Collapsed ) {
        *state = EZ::Expanded;
        tree->expandDataPlank(p); 
        update();
    } else if ( *state == EZ::Expanded ) {
        *state = EZ::Collapsed;
        tree->collapseDataPlank(p);
        update();
    }
}
