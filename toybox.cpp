#include "toybox.h"
#include "errorwindow.h"

#include <QPlainTextEdit>
#include <QLabel>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>

IntHash bytesPerType;

extern QString fortranName;

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

    bytesPerType["logical (kind=1)"] = 1;
    bytesPerType["logical (kind=2)"] = 2;
    bytesPerType["logical (kind=4)"] = 4;
    bytesPerType["logical (kind=8)"] = 8;
    bytesPerType["integer (kind=1)"] = 1;
    bytesPerType["integer (kind=2)"] = 2;
    bytesPerType["integer (kind=4)"] = 4;
    bytesPerType["integer (kind=8)"] = 8;
    bytesPerType["real (kind=4)"] = 4;
    bytesPerType["real (kind=8)"] = 8;
    /*
     *  Set the frame to be raised with a width 4 bevel.
     */
    setFrameStyle(QFrame::Panel | QFrame::Raised);
    setLineWidth(4);

    /*
     *  Create a combobox to select programming language
     */
    languageCombo = new QComboBox;
    languageCombo->addItem("C++");
    languageCombo->addItem("Fortran");
    languageCombo->setCurrentIndex(0);

    language = "C++";

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
    layout->setContentsMargins(10, 10, 10, 10);

    /*
     *  Add the language combobox and a label to the layout
     */

    QHBoxLayout *languageLayout = new QHBoxLayout;
    languageLayout->addWidget(new QLabel(tr("Language")));
    languageLayout->addWidget(languageCombo);
    languageLayout->addStretch();
    layout->addLayout(languageLayout);

    /*
     *  Add the tables to the layout
     */
    layout->addWidget(variableTable);
    layout->addWidget(expressionTable);

    /*
     *  Set the layout for the frame.
     */
    setLayout(layout);

    connect(languageCombo, SIGNAL(activated(QString)), this,
        SLOT(switchLanguage(QString)));
    connect(languageCombo, SIGNAL(activated(QString)), variableTable,
        SLOT(switchLanguage(QString)));
}

/*
 *  Crude size hint.  50 rows is not enough to display much, but it allows
 *  the user to shrink the register window.  300 columns is probably not
 *  enough.
 */
QSize ToyBox::sizeHint() const
{
    return QSize(200, 10);
}

void ToyBox::formatResult(int /* index */)
{
    QComboBox *c = (QComboBox *)sender();
    int row = c->property("row").toInt();
    expressions[row]->setValue();
}

void ToyBox::switchLanguage(QString lang)
{
    language = lang;
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
    if (expression == "") {
        QMessageBox::warning(this, tr("Warning"),
            tr("Your expression is empty."), QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    QString templateFile =
        language == "C++" ? "toybox_template.cpp" : "toybox_template.f";
    QFile in(QString(":/src/%1").arg(templateFile));
    if (!in.open(QFile::ReadOnly)) {
        qDebug() << tr("Could not open") << templateFile;
        return;
    }

    QString t = in.readLine();
    while (t[0] != QChar('/') || t[1] != QChar('/')) {
        code.append(t);
        t = in.readLine();
    }

    QString name, type, val;
    foreach(ToyVariable * v, variables)
    {
        name = v->name->text().trimmed();
        type = v->type->currentText().trimmed();
        val = v->value->text().trimmed();
        if (name.length() > 0 && type.length() > 0) {
            if (language == "C++") {
                code.append(
                    QString("    %1 %2 = %3;\n").arg(type).arg(name).arg(val));
            } else {
                code.append(
                    QString("        %1 :: %2 = %3\n").arg(type).arg(name).arg(
                        val));
            }
        }
    }

    if (language == "C++") {
        code.append(QString("    dump(%1);\n").arg(expression));
    } else {
        code.append(QString("        call dump(%1)\n").arg(expression));
    }

    if (language == "C++") {
        foreach(ToyVariable * v, variables)
        {
            name = v->name->text().trimmed();
            QString t = v->type->currentText().trimmed();
            if (name.length() > 0 && t.length() > 0) {
                if (t == "bool") {
                    code.append(
                        QString("    printf(\"%d\\n\",%1);\n").arg(name));
                } else if (t == "char") {
                    code.append(
                        QString("    printf(\"\'%c\'\\n\",%1);\n").arg(name));
                } else if (t == "signed char") {
                    code.append(
                        QString("    printf(\"%d\\n\",int(%1));\n").arg(name));
                } else if (t == "unsigned char") {
                    code.append(
                        QString("    printf(\"0x%x\\n\",(unsigned int)(%1));\n")
                            .arg(name));
                } else if (t == "short") {
                    code.append(
                        QString("    printf(\"%d\\n\",%1);\n").arg(name));
                } else if (t == "unsigned short") {
                    code.append(
                        QString("    printf(\"0x%x\\n\",%1);\n").arg(name));
                } else if (t == "int") {
                    code.append(
                        QString("    printf(\"%d\\n\",%1);\n").arg(name));
                } else if (t == "unsigned int") {
                    code.append(
                        QString("    printf(\"0x%x\\n\",%1);\n").arg(name));
                } else if (t == "long") {
                    code.append(
                        QString("    printf(\"%ld\\n\",%1);\n").arg(name));
                } else if (t == "unsigned long") {
                    code.append(
                        QString("    printf(\"0x%lx\\n\",%1);\n").arg(name));
                } else if (t == "float" || t == "double") {
                    code.append(
                        QString("    printf(\"%g\\n\",%1);\n").arg(name));
                }
            }
        }
    }

    t = in.readLine();
    while (t != "") {
        code.append(t);
        t = in.readLine();
    }
    in.close();

    QString outFile = language == "C++" ? "ebe_toybox.cpp" : "ebe_toybox.f";
    QFile file(outFile);
    file.open(QFile::WriteOnly);
#if QT_VERSION >= 0x050000
    file.write(code.toLocal8Bit().constData());
#else
    file.write(code.toAscii());
#endif
    file.close();
    QProcess compile(this);
    if (language == "C++") {
        compile.start("g++ -O0 ebe_toybox.cpp -o ebe_toybox");
    } else {
        compile.start(QString("%1 -O0 ebe_toybox.f -o ebe_toybox").arg(fortranName));
    }
    compile.waitForFinished();
    QByteArray errors = compile.readAllStandardError();
    if (errors.length() > 0 && errors.indexOf("-macosx_version") < 0) {
        if (errorWindow.isNull()) errorWindow = new ErrorWindow;
        errorWindow->setWindowTitle("Errors in toy program");
        errorWindow->textEdit->setPlainText(QString(errors));
        errorWindow->show();
        return;
    }
    QString program;
#ifdef Q_WS_WIN
    program = "ebe_toybox.exe";
#else
    program = "./ebe_toybox";
#endif
    compile.start(program);
    compile.waitForFinished();
    t = compile.readLine();
    t = t.trimmed();
    QString data = compile.readLine();
    data = data.trimmed();
    QStringList values = data.split(' ');
    e->setValue(t, values);
    if (language == "C++") {
        foreach(ToyVariable * v, variables)
        {
            name = v->name->text().trimmed();
            type = v->type->currentText().trimmed();
            if (name.length() > 0 && type.length() > 0) {
                QString data = compile.readLine();
                data = data.trimmed();
                v->value->setText(data);
            }
        }
    }
}

void ToyExpression::setValue(QString t, QStringList values)
{
    //qDebug() << "setValue" << t << values;
    if (type->text() != t) {
        QStringList formats;
        format->clear();
        type->setText(t);
        if (t == "char") {
            formats << "character" << "decimal" << "hexadecimal" << "binary";
            format->addItems(formats);
        } else if (t == "unsigned char") {
            formats << "hexadecimal" << "character" << "decimal" << "binary";
            format->addItems(formats);
        } else if (t == "signed char") {
            formats << "decimal" << "character" << "hexadecimal" << "binary";
            format->addItems(formats);
        } else if (t == "bool" || t.indexOf("logical") >= 0) {
            formats << "bool" << "binary";
            format->addItems(formats);
        } else if (t == "float" || t == "double" || t.indexOf("real") >= 0) {
            formats << "decimal" << "hexadecimal" << "binary" << "binary fp"
                << "fields";
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

QString hexadecimal(AllTypes &a, int n)
{
    QString s = "";
    for (int i = 0; i < n; i++) {
        s += QString("%1").arg((int)a.b[n - i - 1], 2, 16, QChar('0'));
        if (i < n - 1) s += " ";
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
    for (int j = 0; j < hex.length(); j++) {
        a.b[j] = hex[j].toInt(&ok, 16);
    }
    if (t == "bool" || t.indexOf("logical") >= 0) {
        if (f == "bool") {
            value->setText(a.b1 ? "true" : "false");
        } else {
            value->setText(binary(a, 1));
        }
    } else if (t == "char" || t == "unsigned char" || t == "signed char") {
        if (f == "hexadecimal") {
            value->setText(hexadecimal(a, 1));
        } else if (f == "binary") {
            value->setText(binary(a, 1));
        } else if (f == "decimal") {
            value->setText(QString("%1").arg(a.i1));
        } else {
            value->setText(QString(QChar(a.c)));
        }
    } else if (t.indexOf("integer") >= 0) {
        if (f == "hexadecimal") {
            value->setText(hexadecimal(a, numBytes));
        } else if (f == "binary") {
            value->setText(binary(a, numBytes));
        } else {
            value->setText(QString("%1").arg(a.i8));
        }
    } else if (t == "short") {
        if (f == "hexadecimal") {
            value->setText(hexadecimal(a, numBytes));
        } else if (f == "binary") {
            value->setText(binary(a, numBytes));
        } else {
            value->setText(QString("%1").arg(a.i2));
        }
    } else if (t == "unsigned short") {
        if (f == "hexadecimal") {
            value->setText(hexadecimal(a, numBytes));
        } else if (f == "binary") {
            value->setText(binary(a, numBytes));
        } else {
            value->setText(QString("%1").arg(a.u2));
        }
    } else if (t == "int") {
        if (f == "hexadecimal") {
            value->setText(hexadecimal(a, numBytes));
        } else if (f == "binary") {
            value->setText(binary(a, numBytes));
        } else {
            value->setText(QString("%1").arg(a.i4));
        }
    } else if (t == "unsigned int") {
        if (f == "hexadecimal") {
            value->setText(hexadecimal(a, numBytes));
        } else if (f == "binary") {
            value->setText(binary(a, numBytes));
        } else {
            value->setText(QString("%1").arg(a.u4));
        }
    } else if (t == "long") {
        if (f == "hexadecimal") {
            value->setText(hexadecimal(a, numBytes));
        } else if (f == "binary") {
            value->setText(binary(a, numBytes));
        } else {
            value->setText(QString("%1").arg(a.i8));
        }
    } else if (t == "unsigned long") {
        if (f == "hexadecimal") {
            value->setText(hexadecimal(a, numBytes));
        } else if (f == "binary") {
            value->setText(binary(a, numBytes));
        } else {
            value->setText(QString("%1").arg(a.u8));
        }
    } else if (t == "float" || t == "real (kind=4)") {
        if (f == "hexadecimal") {
            value->setText(hexadecimal(a, numBytes));
        } else if (f == "binary") {
            value->setText(binary(a, numBytes));
        } else if (f == "binary fp") {
            value->setText(binaryFloat(a));
        } else if (f == "fields") {
            value->setText(fieldsFloat(a));
        } else {
            value->setText(QString("%1").arg(a.f4, 0, 'g', 7));
        }
    } else if (t == "double" || t == "real (kind=8)") {
        if (f == "hexadecimal") {
            value->setText(hexadecimal(a, numBytes));
        } else if (f == "binary") {
            value->setText(binary(a, numBytes));
        } else if (f == "binary fp") {
            value->setText(binaryDouble(a));
        } else if (f == "fields") {
            value->setText(fieldsDouble(a));
        } else {
            value->setText(QString("%1").arg(a.f8, 0, 'g', 15));
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
void ToyBox::setFontHeightAndWidth(int height, int width)
{
    fontHeight = height;
    fontWidth = width;
    variableTable->setFontHeightAndWidth(height, width);
    expressionTable->setFontHeightAndWidth(height, width);
}

ToyVariable::ToyVariable(VariableTable *t, int r)
{
    table = t;
    row = r;
    name = new QLineEdit;
    name->setToolTip(tr("Enter a name for a variable.\n"
        "A value is also required"));
    IdValidator *idValidator = new IdValidator;
    name->setValidator(idValidator);
    table->setCellWidget(row, 0, name);
    type = new QComboBox;
    type->setToolTip(tr("Select a type for your variable. "));
    QStringList types;
    types << " bool" << " char" << " signed char" << " unsigned char"
        << " short" << " unsigned short" << " int" << " unsigned int" << " long"
        << " unsigned long" << " float" << " double";
    type->addItems(types);
    type->setCurrentIndex(6);

    table->setCellWidget(row, 1, type);
    value = new QLineEdit;
    value->setToolTip(tr("Enter a value for the variable."));
    table->setCellWidget(row, 2, value);
    NumberValidator *validator = new NumberValidator;
    value->setValidator(validator);
}

void VariableTable::switchLanguage(QString language)
{
    int i;
    QComboBox *type;
    QStringList types;

    if (language == "C++") {
        types << " bool" << " char" << " signed char" << " unsigned char"
            << " short" << " unsigned short" << " int" << " unsigned int"
            << " long" << " unsigned long" << " float" << " double";
        for (i = 0; i < rowCount(); i++) {
            type = (QComboBox *)cellWidget(i, 1);
            type->clear();
            type->addItems(types);
            type->setCurrentIndex(6);
        }
    } else {
        types << " logical (kind=8)" << " logical (kind=4)"
            << " logical (kind=2)" << " logical (kind=1)" << " integer (kind=8)"
            << " integer (kind=4)" << " integer (kind=2)" << " integer (kind=1)"
            << " real (kind=8)" << " real (kind=4)";
        for (i = 0; i < rowCount(); i++) {
            type = (QComboBox *)cellWidget(i, 1);
            type->clear();
            type->addItems(types);
            type->setCurrentIndex(5);
        }
    }
}

VariableTable::VariableTable(ToyBox *p)
    : QTableWidget()
{
    box = p;
    setColumnCount(3);
    setRowCount(5);
    QStringList headers;
    headers << tr("Variable name") << tr("Type") << tr("Value");
    setHorizontalHeaderLabels(headers);
    ToyVariable *v;
    for (int i = 0; i < 5; i++) {
        v = new ToyVariable(this, i);
        p->variables.push_back(v);
    }
}

void VariableTable::setFontHeightAndWidth(int height, int width)
{
    fontHeight = height;
    fontWidth = width;
    int r, n, length;
    n = rowCount();
    for (r = 0; r < n; r++) {
        setRowHeight(r, height + 6);
    }
    int max = 13;
    for (r = 0; r < n; r++) {
        length = ((QLineEdit *)cellWidget(r, 0))->text().length();
        if (length > max) max = length;
    }
    setColumnWidth(0, (max + 1) * width + 3);
    setColumnWidth(1, 18 * width + 3);
    max = 6;
    for (r = 0; r < n; r++) {
        length = ((QLineEdit *)cellWidget(r, 2))->text().length();
        if (length > max) max = length;
    }
    setColumnWidth(2, (max + 1) * width + 3);
}

ToyExpression::ToyExpression(ExpressionTable *t, int r)
{
    table = t;
    row = r;
    expr = new QLineEdit;
    expr->setToolTip(tr("Enter a C++ expression to evaluate"));
    table->setCellWidget(row, 0, expr);
    button = new QPushButton;
    button->setToolTip(tr("Click this button to evaluate the expression"));
    button->setText("do it");
    button->setProperty("row", r);
    table->setCellWidget(row, 1, button);
    type = new QLineEdit;
    type->setToolTip(tr("The type will be reported back from the evaluation"));
    type->setReadOnly(true);
    table->setCellWidget(row, 2, type);
    format = new QComboBox;
    format->setToolTip(tr("Select a format for the expression value"));
    format->setProperty("row", r);
    table->setCellWidget(row, 3, format);
    value = new QLineEdit;
    value->setToolTip(
        tr("The value will be placed here when you click \"do it\""));
    value->setReadOnly(true);
    table->setCellWidget(row, 4, value);
    connect ( format, SIGNAL(activated(int)),
        table->box, SLOT(formatResult(int)));
    connect(button, SIGNAL(clicked()), table->box, SLOT(computeExpression()));
}

ExpressionTable::ExpressionTable(ToyBox *p)
    : QTableWidget()
{
    box = p;
    setColumnCount(5);
    setRowCount(5);
    QStringList headers;
    headers << tr("Expression") << tr("Execute") << tr("Type") << tr("Format")
        << tr("Result");
    setHorizontalHeaderLabels(headers);
    ToyExpression *e;
    for (int i = 0; i < 5; i++) {
        e = new ToyExpression(this, i);
        p->expressions.push_back(e);
    }
}

void ExpressionTable::setFontHeightAndWidth(int height, int width)
{
    fontHeight = height;
    fontWidth = width;
    int r, n, length;
    n = rowCount();
    for (r = 0; r < n; r++) {
        setRowHeight(r, height + 6);
    }
    int max = 11;
    for (r = 0; r < n; r++) {
        length = ((QLineEdit *)cellWidget(r, 0))->text().length();
        if (length > max) max = length;
    }
    setColumnWidth(0, (max + 1) * width + 3);
    setColumnWidth(1, 7 * width + 3);
    max = 6;
    for (r = 0; r < n; r++) {
        length = ((QLineEdit *)cellWidget(r, 2))->text().length();
        if (length > max) max = length;
    }
    setColumnWidth(2, (max + 1) * width + 3);
}
