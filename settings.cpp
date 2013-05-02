#define SETTINGS_CPP
#include "settings.h"
#include <QDir>
#include <QFile>
#include <QStringList>
#include <QFileDialog>
#include <QDebug>

Settings::Settings()
{
    setDefaults();
    fileName = ".ebe.ini";
}

bool Settings::read()
{
    if ( ! QFile::exists(fileName) ) return false;
    settings = new QSettings ( fileName, QSettings::IniFormat );
    QStringList keys = settings->allKeys();
    int n = keys.count();
    for ( int i = 0; i < n; i++ ) {
        ebe[keys[i]] = settings->value(keys[i]);
        //qDebug() << keys[i] << ebe[keys[i]].toString();
    }
    delete settings;
    return true;
}

void Settings::write()
{
    settings = new QSettings ( fileName, QSettings::IniFormat );
    QStringList keys = ebe.keys();
    int n = keys.count();
    for ( int i = 0; i < n; i++ ) {
        settings->setValue(keys[i],ebe[keys[i]]);
    }
    delete settings;
}

void Settings::setDefaults()
{
    ebe["quit_color"] = "#c00000";
    ebe["run_color"] = "#0000c0";
    ebe["next_color"] = "#0000c0";
    ebe["step_color"] = "#0000c0";
    ebe["continue_color"] = "#00a000";
    ebe["stop_color"] = "#c00000";
    ebe["bg_color"] = "#ffffd8";
    ebe["break_bg"] = "#ff0000";
    ebe["break_fg"] = "#00ffff";
    ebe["button_blue"] = "#000080";
    ebe["button_green"] = "#006000";
    ebe["button_red"] = "#800000";
    ebe["comment_fg"] = "#0000e0";
    ebe["find_bg"] = "#a0a0f0";
    ebe["find_fg"] = "#a00000";
    ebe["fixed_font"] = "Courier";
    ebe["font_size"] = 12;
    ebe["id_fg"] = "#0000a0";
    ebe["illegal_bg"] = "#ffc0ff";
    ebe["illegal_fg"] = "#000000";
    ebe["instruction_fg"] = "#007090";
    ebe["macro_fg"] = "#d00080";
    ebe["next_bg"] = "#b0ffff";
    ebe["next_fg"] = "#ff0000";
    ebe["numbers_bg"] = "#f8f8dc";
    ebe["numbers_fg"] = "#000078";
    ebe["numeric_fg"] = "#80o080";
    ebe["operator_fg"] = "#008860";
    ebe["preprocessor_fg"] = "#c00090";
    ebe["reg_fg"] = "#000080";
    ebe["reg_title_fg"] = "#800000";
    ebe["reserved_fg"] = "#b00000";
    ebe["space_fg"] = "#000000";
    ebe["string_fg"] = "#ffb000";
    ebe["text_bg"] = "#ffffe3";
    ebe["text_fg"] = "#000060";
    ebe["tooltip_bg"] = "#ffffd0";
    ebe["variable_font"] = "Arial";
    ebe["table_bg"] = "#f0ffff";
    ebe["list_bg"] = "#e0f0ff";
    ebe["tree_bg"] = "#e0fafa";

    ebe["edit/tab_width"] = 4;
    ebe["edit/auto_indent"] = true;
#if __linux__
    ebe["linux"] = true;
    ebe["mac"] = false;
    ebe["os"] = "linux";
    ebe["windows"] = false;
    ebe["build/asm"] = "yasm -P ebe.inc -f elf64 -g dwarf2 -l \"$base.lst\" \"$source\"";
    ebe.os = "linux";
#elif __APPLE__
    ebe["linux"] = false;
    ebe["mac"] = true;
    ebe["os"] = "mac";
    ebe["windows"] = false;
    ebe["build/asm"] = "yasm -P ebe.inc -f macho64 -o $base.o -l \"$base.lst\" \"$source\"";
    ebe["build/asmlst"] = "yasm -P ebe.inc -f macho64 -o /dev/null -l \"$base.lst\" \"$source\"";
    ebe.os = "mac";
#else
    ebe["linux"] = false;
    ebe["mac"] = false;
    ebe["os"] = "windows";
    ebe["windows"] = true;
    ebe["build/asm"] = "yasm -P ebe.inc -f win64 -o $base.o -l \"$base.lst\" \"$source\"";
    ebe["build/asmlst"] = "yasm -P ebe.inc -f win64 -o junk_$base.o -l \"$base.lst\" \"$source\"";
    ebe.os = "windows";
#endif
    ebe["prettify"] = "astyle -A3 -s$tab_width -t$tab_width \"$source\"";
    ebe["build/asmld"] = "ld -g -o \"$base\"";
    ebe["build/cc"] = "gcc -g -c -Wfatal-errors -Wall -O0 "
                      "-o \"$base.o\" \"$source\"";
    ebe["build/libs"] = "-lstdc++";
    ebe["build/ccld"] = "gcc -g -o \"$base\" ";
    ebe["build/cpp"] = "g++ -g -c -Wfatal-errors -Wall -O0 "
                      "-o \"$base.o\" \"$source\"";
    ebe["build/cppld"] = "g++ -g -o \"$base\"";
    ebe["build/fortran"] = "gfortran -g -c -Wfatal-errors -Wall -O0 "
                      "-o \"$base.o\" \"$source\"";
    ebe["build/fortranld"] = "gfortran -g -o \"$base\" ";

#ifdef Q_WS_WIN
    ebe["build/obj"] = "o";
#else
    ebe["build/obj"] = "o";
#endif

    ebe["library/path"] = ":/library";

    ebe["buttons/visible"] = false;
    ebe["buttons/icons"] = true;
    ebe["buttons/icon_size"] = 32;

    ebe["toolbars/icon_size"] = 32;

    ebe["console/visible"] = false;

    ebe["data/visible"] = true;

    ebe["register/fg"] = "#c09000";
    ebe["register/visible"] = false;

    ebe["float/visible"] = false;

    ebe["project/visible"] = false;
    ebe["project/auto_open"] = true;

    ebe["terminal/visible"] = true;

    ebe["command/visible"] = true;

    ebe["backtrace/visible"] = false;

    ebe["toybox/visible"] = true;

    ebe["bitbucket/visible"] = false;

    ebe["tooltips/visible"] = true;

    ebe["complete/minimum"] = 4;

    ebe["xmm/reverse"] = false;
}

SettingsHash newSettings;

SettingsDialog::SettingsDialog()
: QDialog()
{
    setObjectName("Settings dialog");
    setWindowTitle("Edit Settings");
    setModal(true);

    newSettings = ebe;

    QPoint pos = QCursor::pos();
    int y = pos.y()-300;
    if ( y < 100 ) y = 100;
    pos.setY(y);
    move ( pos );

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setSpacing(1);
    mainLayout->setContentsMargins(3,3,3,3);

    QVBoxLayout *columnLayout;
    SettingsFrame *frame;
    
    columnLayout = new QVBoxLayout;
    frame = new SettingsFrame("Debug colors");

    frame->addColorSetter("Background", "bg_color");
    frame->addColorSetter("Breakpoint fg", "break_fg");
    frame->addColorSetter("Breakpoint bg", "break_bg");
    frame->addColorSetter("Find fg", "find_fg");
    frame->addColorSetter("Find bg", "find_bg");
    frame->addColorSetter("Line number fg", "numbers_fg");
    frame->addColorSetter("Line number bg", "numbers_bg");
    frame->addColorSetter("Next line fg", "next_fg");
    frame->addColorSetter("Next line bg", "next_bg");
    frame->addStretch();
    columnLayout->addWidget(frame);
    //mainLayout->addLayout(columnLayout);

    //columnLayout = new QVBoxLayout;
    frame = new SettingsFrame("Other colors");

    frame->addColorSetter("Register","reg_fg");
    frame->addColorSetter("Register title","reg_title_fg");

    frame->addColorSetter("Tooltip","tooltip_bg");
    frame->addColorSetter("Table","table_bg");
    frame->addColorSetter("List","list_bg");
    frame->addColorSetter("Tree","tree_bg");
    frame->addStretch();
    columnLayout->addWidget(frame);
    mainLayout->addLayout(columnLayout);

    columnLayout = new QVBoxLayout;
    frame = new SettingsFrame("Highlight colors");

    frame->addColorSetter("Comment", "comment_fg");
    frame->addColorSetter("Identifier", "id_fg");
    frame->addColorSetter("Reserved", "reserved_fg");
    frame->addColorSetter("String", "string_fg");
    frame->addColorSetter("Number", "numeric");
    frame->addColorSetter("Operator", "operator_fg");
    frame->addColorSetter("Instruction", "instruction_fg");
    frame->addColorSetter("Macro", "macro_fg");
    frame->addColorSetter("Preprocessor","preprocessor_fg");
    frame->addColorSetter("Illegal fg", "illegal_fg");
    frame->addColorSetter("Illegal bg", "illegal_bg");
    frame->addStretch();
    columnLayout->addWidget(frame);

    QFrame *f = new QFrame;
    f->setFrameStyle ( QFrame::Panel | QFrame::Raised );
    f->setLineWidth(3);
    QVBoxLayout *vbox = new QVBoxLayout;
    QPushButton *save = new QPushButton(tr("Save"));
    QPushButton *cancel = new QPushButton(tr("Cancel"));
    vbox->addStretch();
    vbox->addWidget(save);
    vbox->addStretch();
    vbox->addWidget(cancel);
    vbox->addStretch();
    f->setLayout(vbox);
    columnLayout->addWidget(f);
    mainLayout->addLayout(columnLayout);
    connect ( cancel, SIGNAL(clicked()), this, SLOT(reject()) );
    connect ( save, SIGNAL(clicked()), this, SLOT(save()) );

    columnLayout = new QVBoxLayout;
    Spinner *spin;
    ComboBox *box;
    QStringList strings;
    frame = new SettingsFrame("Options");
    spin = frame->addSpinner("Tab spacing","edit/tab_width");
    spin->setRange(2,16);
    spin = frame->addSpinner("Completion minimum","complete/minimum");
    spin->setRange(4,32);
    box = frame->addComboBox("Toolbar icon size","toolbars/icon_size");
    strings << "16" << "24" << "32" << "48";
    box->setChoices(strings);
    box = frame->addComboBox("Debug button icon size","buttons/icon_size");
    box->setChoices(strings);
    frame->addCheckBox("Auto-indent","edit/auto_indent");
    frame->addCheckBox("Auto-open project files","edit/auto_indent");
    frame->addCheckBox("Display debug buttons","buttons/visible");
    frame->addCheckBox("Icons on debug buttons","buttons/icons");
    frame->addCheckBox("XMM Reverse","xmm/reverse");
    frame->addStretch();
    columnLayout->addWidget(frame);
    //mainLayout->addLayout(columnLayout);

    //columnLayout = new QVBoxLayout;
    frame = new SettingsFrame("External commands");
    frame->addLineEdit("Prettify","prettify");
    frame->addLineEdit("Assemble","build/asm");
    frame->addLineEdit("Link asm","build/asmld");
    frame->addLineEdit("C Compile","build/cc");
    frame->addLineEdit("Link C","build/ccld");
    frame->addLineEdit("Compile C++","build/cpp");
    frame->addLineEdit("Link C","build/cppld");
    frame->addLineEdit("Compile Fortran","build/fortran");
    frame->addLineEdit("Link Fortran","build/fortranld");
    frame->addStretch();
    columnLayout->addWidget(frame);
    mainLayout->addLayout(columnLayout);

    setLayout(mainLayout);

}

void SettingsDialog::save()
{
    //qDebug() << "Saving";
    ebe = newSettings;
    accept();
}

void SettingsFrame::addColorSetter ( QString label, QString var)
{
    int row = grid->rowCount();
    grid->addWidget ( new QLabel(label), row, 0 );
    ColorButton *button = new ColorButton(var);
    connect(button,SIGNAL(clicked()), button, SLOT(setColor()));
    grid->addWidget(button, row, 1);
}

void SettingsFrame::addStretch()
{
    layout->addStretch();
}

SettingsFrame::SettingsFrame(QString title)
: QFrame()
{
    setFrameStyle ( QFrame::Panel | QFrame::Raised );
    setLineWidth(3);

    layout = new QVBoxLayout;
    grid = new QGridLayout;
    layout->setSpacing(3);
    layout->setContentsMargins(10,10,10,10);
    QLabel *p = new QLabel(title);
    layout->addWidget(p);
    layout->setAlignment(p,Qt::AlignCenter);
    p = new QLabel;
    QPixmap *pm = new QPixmap(3*title.length()*ebe["font_size"].toInt()/8,2);
    pm->fill("black");
    p->setPixmap(*pm);
    layout->addWidget(p);
    layout->setAlignment(p,Qt::AlignCenter);
    layout->addLayout(grid);
    setLayout(layout);
}

ColorButton::ColorButton(QString v)
: QPushButton()
{
    var = v;
    int chipSize = 1.2 * ebe["font_size"].toInt();
    setFlat(true);
    pm = new QPixmap(chipSize,chipSize);
    pm->fill(QColor(ebe[var].toString()));
    setIconSize(QSize(chipSize,chipSize));
    setIcon(*pm);
    setFixedSize(chipSize,chipSize);
}

void ColorButton::setColor()
{
    QColorDialog *dialog = new QColorDialog;
    dialog->setCurrentColor(QColor(ebe[var].toString()));
    dialog->setModal(true);
    if ( dialog->exec() ) {
        QColor color = dialog->currentColor();
        QString c;
        pm->fill(color);
        setIcon(*pm);
        c.sprintf("#%02x%02x%02x",color.red(),color.green(),color.blue());
        newSettings[var] =  c;
    }
}

Spinner::Spinner(QString v)
: QSpinBox()
{
    var = v;
    setValue(ebe[var].toInt());
}

Spinner * SettingsFrame::addSpinner(QString label, QString var )
{
    int row = grid->rowCount();
    grid->addWidget ( new QLabel(label), row, 0 );
    Spinner *spinner = new Spinner(var);
    connect(spinner,SIGNAL(valueChanged(int)),
            spinner, SLOT(saveValue(int)));
    grid->addWidget(spinner, row, 1);
    return spinner;
}

void Spinner::saveValue ( int v )
{
    newSettings[var] = v;
}

LineEdit::LineEdit(QString v)
: QLineEdit()
{
    var = v;
    setText(ebe[var].toString());
    setFixedWidth(ebe["font_size"].toInt()*38);
}

void SettingsFrame::addLineEdit(QString label, QString var )
{
    int row = grid->rowCount();
    grid->addWidget ( new QLabel(label), row, 0 );
    LineEdit *edit = new LineEdit(var);
    connect(edit,SIGNAL(textChanged(QString)),
            edit, SLOT(saveValue(QString)));
    grid->addWidget(edit, row, 1);
}

void LineEdit::saveValue ( QString v )
{
    newSettings[var] = v;
}

CheckBox::CheckBox(QString v)
: QCheckBox()
{
    var = v;
    setChecked(ebe[var].toBool());
}

void SettingsFrame::addCheckBox(QString label, QString var )
{
    int row = grid->rowCount();
    grid->addWidget ( new QLabel(label), row, 0 );
    CheckBox *box = new CheckBox(var);
    connect(box,SIGNAL(stateChanged(int)),
            box, SLOT(saveValue(int)));
    grid->addWidget(box, row, 1);
}

void CheckBox::saveValue ( int /* v */)
{
    newSettings[var] = isChecked();
}


ComboBox::ComboBox(QString v)
: QComboBox()
{
    var = v;
}

void ComboBox::setChoices(QStringList items)
{
    addItems(items);
    int i = items.indexOf(ebe[var].toString());
    if ( i < 0 ) i = 0;
    setCurrentIndex(i);
}

ComboBox * SettingsFrame::addComboBox(QString label, QString var )
{
    int row = grid->rowCount();
    grid->addWidget ( new QLabel(label), row, 0 );
    ComboBox *box = new ComboBox(var);
    connect(box,SIGNAL(activated(QString)),
            box, SLOT(saveValue(QString)));
    grid->addWidget(box, row, 1);
    return box;
}

void ComboBox::saveValue ( QString v )
{
    newSettings[var] = v;
}

