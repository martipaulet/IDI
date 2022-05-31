#include <QPushButton>
class MyQPushButton: public QPushButton
{
    Q_OBJECT 
    public:
        MyQPushButton(QWidget *parent);

    signals:
        void startpressed();
        void increase(int);

    public slots: 
        void ChangeColor();
        void MakeStart();
        void Reset();
        void Reset2();

    private:
        static int c1; 
        static int c2;
        static int c3;
        static int trio;

};