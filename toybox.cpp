#include "toybox.h"
#include "errorwindow.h"

QHash<QString,int> bytesPerType;

/*
 *  Constructor
 */
ToyBox::ToyBox(QWidget *parent)
: QFrame(parent)
{
/*
 *  For the mainwindow's saveState/restoreState and
 *  saveGeometry/restoreGeometry to work properly each dock widget and toolbar
 *  needs a unique object name set.
 */
    setObjectName("Toy Box");

    bytesPerType["bool"] = 1;
    bytesPerType["char"] = 1;
    bytesPerType["signed char"] = 1;
    bytesPerType["unsigned char"] = 1;
    bytesPerType["short"] = 2;
    bytesPerType["signed short"] = 2;
    bytesPerType["unsigned short"] = 2;
    bytesPerType["int"] = 4;
    bytesPerType["signed int"] = 4;
    bytesPerType["unsigned int"] = 4;
#ifdef Q_OS_WIN
    bytesPerType["long"] = 4;
    bytesPerType["signed long"] = 4;
    bytesPerType["unsigned long"] = 4;
#else
    bytesPerType["long"] = 8;
    bytesPerType["signed long"] = 8;
    bytesPerType["unsigned long"] = 8;
#endif
    bytesPerType["float"] = 4;
    bytesPerType["double"] = 8;
/*
 *  Set the frame to be raised with a width 4 bevel.
 */
    setFrameStyle ( QFrame::Panel | QFrame::Raised );
    setLineWidth(4);

/*
 *  Create a table to display the variables
 */
    variableTable = new VariableTable(this);

/*
 *  Create a table to display the expressions
 */
    expressionTable = new ExpressionTable(this);

/*
 *  We need a layout for the tables
 */
    QVBoxLayout *layout = new QVBoxLayout;

/*
 *  Leave 10 pixels all around the table
 */
    layout->setContentsMargins(10,10,10,10);

/*
 *  Add the tables to the layout and set the layout for the frame.
 */
    layout->addWidget(variableTable);
    layout->addWidget(expressionTable);
    setLayout(layout);
}

/*
 *  Crude size hint.  50 rows is not enough to display much, but it allows
 *  the user to shrink the register window.  300 columns is probably not
 *  enough.
 */
QSize ToyBox::sizeHint() const
{
    return QSize(300,50);
}

void ToyBox::formatResult(int /* index */)
{
    QComboBox *c = (QComboBox *)sender();
    int row = c->property("row").toInt();
    expressions[row]->setValue();
}

void ToyBox::computeExpression()
{
    int i;
    code = "";
    QPushButton *p = (QPushButton*)sender();
    i = p->property("row").toInt();
    QString expression;
    ToyExpression *e = expressions[i];
    expression = e->expr->text().trimmed();
    if ( expression == "" ) {
        QMessageBox::warning(this,tr("Warning"),
            tr("Your expression is empty."),
            QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    code.append("#include <cstdio>\n"
                "#include <typeinfo>\n"
                "#include <cmath>\n"
                "int main()\n"
                "{\n");

    QString name, type, val;
    foreach ( ToyVariable *v, variables ) {
        name = v->name->text().trimmed();
        type = v->type->currentText().trimmed();
        val = v->value->text().trimmed();
        if ( name.length() > 0 && type.length() > 0 ) {
            code.append(QString("  %1 %2 = %3;\n").arg(type).arg(name).arg(val));
        }
    }
    code.append(QString("  typeof(%1) __ebe_v;\n").arg(expression));
    code.append(QString("  __ebe_v = %1;\n").arg(expression));
    QStringList types;
    types << "bool"
          << "char"  << "signed char"   << "unsigned char"
          << "short" << "unsigned short"
          << "int"   << "unsigned int"
          << "long"  << "unsigned long"
          << "float" << "double";
    foreach ( QString s, types ) {
        code.append(
           QString("  if ( typeid(__ebe_v) == typeid(%1) ) {\n").arg(s) +
           QString("     printf(\"%1\\n\");\n").arg(s) +
           QString("     goto __wrap_up;\n") +
           QString("}\n"));
    }
    code.append("__wrap_up:\n");
    code.append("  for ( int __ebe_i=0; __ebe_i < sizeof(__ebe_v); __ebe_i++ )\n");
    code.append("    printf(\"%02x \",((unsigned char *)&__ebe_v)[__ebe_i]);\n");
    code.append("  printf(\"\\n\");\n");
    code.append("  return 0;\n"
                "}\n");
    QFile file("ebe_toybox.cpp");
    file.open(QFile::WriteOnly);
    file.write(code.toAscii());
    file.close();
    QProcess compile(this);
    compile.start("g++ -O0 ebe_toybox.cpp -o ebe_toybox");
    compile.waitForFinished();
    QByteArray errors = compile.readAllStandardError();
    if ( errors.length() > 0 ) {
        if ( errorWindow.isNull() ) errorWindow = new ErrorWindow;
        errorWindow->setWindowTitle("Errors in toy program");
        errorWindow->textEdit->setPlainText(QString(errors));
        errorWindow->show();
        return;
    }
    compile.start("./ebe_toybox");
    compile.waitForFinished();
    QString t = compile.readLine();
    t = t.trimmed();
    QString data = compile.readLine();
    data = data.trimmed();
    QStringList values = data.split(' ');
    e->setValue(t,values);
}

void ToyExpression::setValue( QString t, QStringList values )
{
    if ( type->text() != t ) {
        QStringList formats;
        format->clear();
        type->setText(t);
        if ( t == "char" ) {
            formats << "char" << "decimal" << "hexadecimal" << "binary";
            format->addItems(formats);
        } else if ( t == "unsigned char" ) {
            formats << "hexadecimal" << "char" << "decimal" << "binary";
            format->addItems(formats);
        } else if ( t == "signed char" ) {
            formats << "decimal" << "char" << "hexadecimal" << "binary";
            format->addItems(formats);
        } else if ( t == "bool" ) {
            formats << "bool" << "binary";
            format->addItems(formats);
        } else {
            formats << "decimal" << "hexadecimal" << "binary";
            format->addItems(formats);
        }
        format->setCurrentIndex(0);
    }
    hex = values;
    setValue();
}

QString binary(AllTypes &a, int n)
{
    QString s="";
    int mask;
    for ( int i = 0; i < n; i++ ) {
        mask = 0x80;
        for ( int b = 0; b < 8; b++ ) {
            s += a.b[n-i-1] & mask ? "1" : "0";
            mask >>= 1;
        }
        if ( i < n-1 ) s+= " ";
    }
    return s;
}


QString hexadecimal(AllTypes &a, int n)
{
    QString s="";
    for ( int i = 0; i < n; i++ ) {
        s += QString("%1").arg((int)a.b[n-i-1],2,16,QChar('0'));
        if ( i < n-1 ) s+= " ";
    }
    return s;
}

void ToyExpression::setValue()
{
    bool ok;
    QString f = format->currentText();
    QString t = type->text();
    AllTypes a;
    int numBytes = bytesPerType[t];
    a.i8 = 0;
    for ( int j = 0; j < hex.length(); j++ ) {
        a.b[j] = hex[j].toInt(&ok,16);
    }
    if ( t == "bool" ) {
        if ( f == "bool" ) {
            value->setText(a.b1 ? "true" : "false" );
        } else {
            value->setText(binary(a,1));
        }
    } else if ( t == "char" || t == "unsigned char" || t == "signed char" ) {
        if ( f == "hexadecimal" ) {
            value->setText(hexadecimal(a,1));
        } else if ( f == "binary" ) {
            value->setText(binary(a,1));
        } else if ( f == "decimal" ) {
            value->setText(QString("%1").arg(a.i1));
        } else {
            value->setText(QString(QChar(a.c)));
        }
    } else if ( t == "short" ) {
        if ( f == "hexadecimal" ) {
            value->setText(hexadecimal(a,numBytes));
        } else if ( f == "binary" ) {
            value->setText(binary(a,numBytes));
        } else {
            value->setText(QString("%1").arg(a.i2));
        }
    } else if ( t == "unsigned short" ) {
        if ( f == "hexadecimal" ) {
            value->setText(hexadecimal(a,numBytes));
        } else if ( f == "binary" ) {
            value->setText(binary(a,numBytes));
        } else {
            value->setText(QString("%1").arg(a.u2));
        }
    } else if ( t == "int" ) {
        if ( f == "hexadecimal" ) {
            value->setText(hexadecimal(a,numBytes));
        } else if ( f == "binary" ) {
            value->setText(binary(a,numBytes));
        } else {
            value->setText(QString("%1").arg(a.i4));
        }
    } else if ( t == "unsigned int" ) {
        if ( f == "hexadecimal" ) {
            value->setText(hexadecimal(a,numBytes));
        } else if ( f == "binary" ) {
            value->setText(binary(a,numBytes));
        } else {
            value->setText(QString("%1").arg(a.u4));
        }
    } else if ( t == "long" ) {
        if ( f == "hexadecimal" ) {
            value->setText(hexadecimal(a,numBytes));
        } else if ( f == "binary" ) {
            value->setText(binary(a,numBytes));
        } else {
            value->setText(QString("%1").arg(a.i8));
        }
    } else if ( t == "unsigned long" ) {
        if ( f == "hexadecimal" ) {
            value->setText(hexadecimal(a,numBytes));
        } else if ( f == "binary" ) {
            value->setText(binary(a,numBytes));
        } else {
            value->setText(QString("%1").arg(a.u8));
        }
    } else if ( t == "float" ) {
        if ( f == "hexadecimal" ) {
            value->setText(hexadecimal(a,numBytes));
        } else if ( f == "binary" ) {
            value->setText(binary(a,numBytes));
        } else {
            value->setText(QString("%1").arg(a.f4,0,'g',7));
        }
    } else if ( t == "double" ) {
        if ( f == "hexadecimal" ) {
            value->setText(hexadecimal(a,numBytes));
        } else if ( f == "binary" ) {
            value->setText(binary(a,numBytes));
        } else {
            value->setText(QString("%1").arg(a.f8,0,'g',15));
        }
    } else {
        qDebug() << "unknown type" << t;
        return;
    }
}

/*
 *  This function sets the row heights and column widths based on
 *  the contents of the table and the current font size.
 */
void ToyBox::setFontHeightAndWidth ( int height, int width )
{
    fontHeight = height;
    fontWidth = width;
    variableTable->setFontHeightAndWidth(height,width);
    expressionTable->setFontHeightAndWidth(height,width);
}

ToyVariable::ToyVariable ( VariableTable *t, int r )
{
    table = t;
    row = r;
    name = new QLineEdit;
    IdValidator *idValidator = new IdValidator;
    name->setValidator(idValidator);
    table->setCellWidget(row,0,name);
    type = new QComboBox;
    QStringList types;
    types << " bool"
          << " char"  << " signed char"   << " unsigned char"
          << " short" << " unsigned short"
          << " int"   << " unsigned int"
          << " long"  << " unsigned long"
          << " float" << " double";
    type->addItems(types);
    type->setCurrentIndex(6);

    table->setCellWidget(row,1,type);
    value = new QLineEdit;
    table->setCellWidget(row,2,value);
    NumberValidator *validator = new NumberValidator;
    value->setValidator(validator);
}

VariableTable::VariableTable(ToyBox *p)
: QTableWidget()
{
    box = p;
    setColumnCount(3);
    setRowCount(5);
    QStringList headers;
    headers << "Variable name" << "Type" << "Value";
    setHorizontalHeaderLabels(headers);
    ToyVariable *v;
    for ( int i=0; i < 5; i++ ) {
        v = new ToyVariable(this,i);
        p->variables.push_back(v);
    }
}

void VariableTable::setFontHeightAndWidth ( int height, int width )
{
    fontHeight = height;
    fontWidth = width;
    int r, n, length;
    n = rowCount();
    for ( r = 0; r < n; r++ ) {
        setRowHeight(r,height+6);
    }
    int max = 13;
    for ( r = 0; r < n; r++ ) {
        length = ((QLineEdit *)cellWidget(r,0))->text().length();
        if ( length > max ) max = length;
    }
    setColumnWidth(0,(max+1)*width+3);
    setColumnWidth(1,18*width+3);
    max = 6;
    for ( r = 0; r < n; r++ ) {
        length = ((QLineEdit *)cellWidget(r,2))->text().length();
        if ( length > max ) max = length;
    }
    setColumnWidth(2,(max+1)*width+3);
}

ToyExpression::ToyExpression(ExpressionTable *t, int r)
{
    table = t;
    row = r;
    expr = new QLineEdit;
    table->setCellWidget(row,0,expr);
    button = new QPushButton;
    button->setText("do it");
    button->setProperty("row",r);
    table->setCellWidget(row,1,button);
    type = new QLineEdit;
    type->setReadOnly(true);
    table->setCellWidget(row,2,type);
    format = new QComboBox;
    format->setProperty("row",r);
    table->setCellWidget(row,3,format);
    value = new QLineEdit;
    value->setReadOnly(true);
    table->setCellWidget(row,4,value);
    connect ( format,  SIGNAL(activated(int)),
              table->box, SLOT(formatResult(int)));
    connect ( button,  SIGNAL(clicked()),
              table->box, SLOT(computeExpression()));
}

ExpressionTable::ExpressionTable(ToyBox *p)
: QTableWidget()
{
    box = p;
    setColumnCount(5);
    setRowCount(5);
    QStringList headers;
    headers << "Expression" << "Execute" << "Type" << "Format" << "Result";
    setHorizontalHeaderLabels(headers);
    ToyExpression *e;
    for ( int i=0; i < 5; i++ ) {
        e = new ToyExpression(this,i);
        p->expressions.push_back(e);
    }
}

void ExpressionTable::setFontHeightAndWidth ( int height, int width )
{
    fontHeight = height;
    fontWidth = width;
    int r, n, length;
    n = rowCount();
    for ( r = 0; r < n; r++ ) {
        setRowHeight(r,height+6);
    }
    int max = 11;
    for ( r = 0; r < n; r++ ) {
        length = ((QLineEdit *)cellWidget(r,0))->text().length();
        if ( length > max ) max = length;
    }
    setColumnWidth(0,(max+1)*width+3);
    setColumnWidth(1,7*width+3);
    max = 6;
    for ( r = 0; r < n; r++ ) {
        length = ((QLineEdit *)cellWidget(r,2))->text().length();
        if ( length > max ) max = length;
    }
    setColumnWidth(2,(max+1)*width+3);
}

NumberValidator::NumberValidator()
: QValidator()
{
}

QValidator::State NumberValidator::validate(QString &s, int & /* pos */ ) const
{
    QString t = s.trimmed();
    if ( t == "" ) return QValidator::Intermediate;
    if ( t == "-" ) return QValidator::Intermediate;
    if ( t == "'" ) return QValidator::Intermediate;
    if ( t == "+" ) return QValidator::Intermediate;
    if ( t == "0x" ) return QValidator::Intermediate;
    if ( t == "0b" ) return QValidator::Intermediate;
    QRegExp rx1(QString("[\\+\\-]?[0-9]+"));
    if ( rx1.exactMatch(t) ) return QValidator::Acceptable;
    QRegExp rx2(QString("0x[0-9a-fA-F]+"));
    if ( rx2.exactMatch(t) ) return QValidator::Acceptable;
    QRegExp rx3(QString("0b[01]+"));
    if ( rx3.exactMatch(t) ) return QValidator::Acceptable;
    QRegExp rx4(QString("[\\+\\-]?[0-9]+\\.[0-9]+"));
    if ( rx4.exactMatch(t) ) return QValidator::Acceptable;
    QRegExp rx5(QString("[\\+\\-]?[0-9]*\\.[0-9]*"));
    if ( rx5.exactMatch(t) ) return QValidator::Intermediate;
    QRegExp rx6(QString("'."));
    if ( rx6.exactMatch(t) ) return QValidator::Intermediate;
    QRegExp rx7(QString("'.'"));
    if ( rx7.exactMatch(t) ) return QValidator::Acceptable;
    return QValidator::Invalid;
}

IdValidator::IdValidator()
: QValidator()
{
}

QValidator::State IdValidator::validate(QString &s, int & /* pos */ ) const
{
    QString t = s.trimmed();
    if ( t == "" ) return QValidator::Intermediate;
    QRegExp rx1(QString("[a-zA-Z][a-zA-Z0-9_]*"));
    if ( rx1.exactMatch(t) ) return QValidator::Acceptable;
    return QValidator::Invalid;
}
