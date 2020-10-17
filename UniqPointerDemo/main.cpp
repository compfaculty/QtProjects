#include <QCoreApplication>
#include <QDebug>
class MyClass
{
public:
    MyClass(QString name){
        Name = name;
        qDebug() << "Start INit" << Name;
    }
    ~MyClass(){
        qDebug() << "Finish" << Name;
    }
    void PrintName(){
        qDebug() << "Hello" << Name;
    }
private:
    QString Name;
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    {
    std::unique_ptr<MyClass> mc(new MyClass("TEst"));
    for (int i = 0;i < 5; i++) {
        mc->PrintName();
    }
    }
    return a.exec();
}
