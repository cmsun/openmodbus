#include <stdarg.h>
#include <QDebug>
#include <QMessageBox>

extern "C" void qprintf(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    qDebug() << QString().vsprintf(format, ap); 
    va_end(ap);
}

extern "C" void qmsg(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    QMessageBox::information(NULL, "", QString().vsprintf(format, ap)); 
    va_end(ap);
}
