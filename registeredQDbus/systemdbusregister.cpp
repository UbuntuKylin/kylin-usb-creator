#include "systemdbusregister.h"
#include <polkitqt1-authority.h>
#include <QDBusConnection>
#include <QDBusError>
#include <QDBusMessage>
#include <QDebug>

using namespace PolkitQt1;

SystemDbusRegister::SystemDbusRegister()
{

}

QString SystemDbusRegister::GetComputerInfo()
{
    QByteArray ba;
    FILE * fp = NULL;
    char cmd[128];
    char buf[1024];
    sprintf(cmd, "dmidecode -t system");

    if ((fp = popen(cmd, "r")) != NULL){
        rewind(fp);
        while (!feof(fp)) {
            fgets(buf, sizeof (buf), fp);
            ba.append(buf);
        }
        pclose(fp);
        fp = NULL;
    }
    return QString(ba);
}
