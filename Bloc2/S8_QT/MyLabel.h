#include <QLabel>
class MyLabel: public QLabel
{
    Q_OBJECT 
    public:
        MyLabel (QWidget *parent);
    public slots: 
        void  truncarLabel(int);
        void TextTruncat(QString);
    private:
        int pos;
        QString prev_text;
};