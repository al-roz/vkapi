#pragma once

#include <QtWidgets/QMainWindow>
#include <QmessageBox>
#include <Qstring>
#include <QInputDialog>
#include <qvector.h>
#include <qtimer.h>

#include "ui_vkapi.h"
#include "api.h"

using json = nlohmann::json;

class DialogInfo {
public:
    //bool is_chat = false;
    int chat_id;
    int from_id;
    std::string body;
    std::string title = "";
};


class vkapi : public QMainWindow
{
    Q_OBJECT

public:
    vkapi(QWidget *parent = Q_NULLPTR);

private slots:
    void takeLogin();
    void timerRound();

private:
    QTimer* timer;
    Ui::vkapiClass ui;
    VK::Client api;
    static std::string fa2_callback();
    static std::string captcha_callback(const std::string& captcha_sid);
    QVector<DialogInfo> get_convert();
    std::string getUserName(const unsigned int& id);
};
