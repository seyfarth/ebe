    QFile::remove("hello.c");
    QFile::copy(":/src/c/hello.c", "hello.c");
    QFile::setPermissions("hello.c", QFile::ReadOwner | QFile::WriteOwner);
