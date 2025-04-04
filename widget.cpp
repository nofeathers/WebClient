#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    QObject::connect(&tcpsocket_, &QAbstractSocket::connected, this, &Widget::doConnected);
    QObject::connect(&tcpsocket_, &QAbstractSocket::disconnected, this, &Widget::doDisconnected);
    QObject::connect(&tcpsocket_, &QAbstractSocket::readyRead, this, &Widget::doReadyRead);

    QObject::connect(&sslsocket_, &QAbstractSocket::connected, this, &Widget::doConnected);
    QObject::connect(&sslsocket_, &QAbstractSocket::disconnected, this, &Widget::doDisconnected);
    QObject::connect(&sslsocket_, &QAbstractSocket::readyRead, this, &Widget::doReadyRead);

    butstate();
}

Widget::~Widget() {
    delete ui;
}

void Widget::butstate() {
    bool connected = ui->rbHttp->isChecked()
    ? tcpsocket_.isOpen()
    : (sslsocket_.isOpen() || sslsocket_.isEncrypted());

    ui->pbConnect->setEnabled(!connected);
    ui->pbDisconnect->setEnabled(connected);
}

void Widget::doConnected() {
    QString msg = "Connected\r\n";
    ui->pteMessge->insertPlainText(msg);
    butstate();
}

void Widget::doDisconnected() {
    QString msg = "Disconnected\r\n";
    ui->pteMessge->insertPlainText(msg);
    butstate();
}

void Widget::doReadyRead() {
    QByteArray ba;
    if(ui->rbHttp->isChecked()){
        ba = tcpsocket_.readAll();
    } else {
        ba = sslsocket_.readAll();
    }
   ui->pteMessge->insertPlainText(ba);
    butstate();
}

void Widget::on_pbConnect_clicked() {
    if(ui->rbHttp->isChecked()) {
        tcpsocket_.connectToHost(ui->leHost->text(), ui->lePort->text().toUShort());
    } else {
        sslsocket_.connectToHostEncrypted(ui->leHost->text(), ui->lePort->text().toUShort());
    }
    butstate();
}

void Widget::on_pbDisconnect_clicked() {
    if(ui->rbHttp->isChecked()){
        tcpsocket_.close();
    }else {
        sslsocket_.close();
    }
    butstate();
}


void Widget::on_pbClear_clicked() {
    ui->pteMessge->clear();
}


void Widget::on_pbSend_clicked() {
    if(ui->rbHttp->isChecked()){
        tcpsocket_.write(ui->pteSend->toPlainText().toUtf8());
    } else {
        sslsocket_.write(ui->pteSend->toPlainText().toUtf8());
    }
}


void Widget::on_rbHttp_clicked() {

}


void Widget::on_rbHttps_clicked() {

}

