#include "vkapi.h"
#include "api.h"

vkapi::vkapi(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    ui.messagesText->setVisible(false);
    this->timer = new QTimer();
    this->timer->setInterval(1000);
    api.set_fa2_callback(fa2_callback);
    api.set_cap_callback(captcha_callback);

    connect(ui.LogButton, SIGNAL(released()), this, SLOT(takeLogin()));
    connect(this->timer, SIGNAL(timeout()), this, SLOT(timerRound()));
}



void vkapi::takeLogin()
{
    QString pass, login;
    pass = ui.passwordEdit->text();
    login = ui.LoginEdit->text();
    if (api.auth(login.toStdString(), pass.toStdString()))
    {
        delete ui.LoginEdit;
        delete ui.LogButton;
        delete ui.passwordEdit;
        delete ui.label;
        delete ui.label_2;
        ui.messagesText->setVisible(true);
        this->timer->start();
        
    }
}

void vkapi::timerRound()
{
    ui.messagesText->clear();
    QVector<DialogInfo> messegas = get_convert();

    for (auto i : messegas)
    {
        QString tmp = QString::fromStdString(i.title) + ":" + QString::fromStdString(i.body);
        ui.messagesText->addItem(tmp);
    }
   
}

std::string vkapi::fa2_callback()
{
    bool flag;
    QString fa2_code = QInputDialog::getText(0, "Two-factor Authorization", "Code:", QLineEdit::Normal, "", &flag);
    if (flag)
    {
        return fa2_code.toStdString();
    }
    return "";
}

std::string vkapi::captcha_callback(const std::string& captcha_sid)
{
    std::string result;
    std::cout << captcha_callback;
    std::cin >> result;
    return result;
}

QVector<DialogInfo> vkapi::get_convert()
{
    QVector<DialogInfo> result;
    VK::params_map params =
    {
        {"count", "30"},
        {"filter", "unread"},
        {"extended", "1"}
    };
    
    json jres = api.call("messages.getConversations",params);
    DialogInfo chat;
    jres = jres.at("response").get<json>().at("items");
    for (auto i : jres)
    {
        chat.chat_id = i.at("conversation").get<json>().at("peer").get<json>().at("id").get<unsigned int>();
        chat.from_id = i.at("last_message").get<json>().at("from_id").get<unsigned int>();
        chat.body = i.at("last_message").get<json>().at("text").get<std::string>();
        //chat.title += i.at("profiles").get<json>().at("first_name").get<std::string>();
        //chat.title += i.at("profiles").get<json>().at("last_name").get<std::string>();
        chat.title = getUserName(chat.from_id);
        result.push_back(chat);
    }   
    return result;
}

std::string vkapi::getUserName(const unsigned int& id)
{
    VK::params_map params = {
    {"user_ids", std::to_string(id)},
    };

    json jres = api.call("users.get", params);
    if (jres == nullptr || jres.find("error") != jres.end()) {
        return "";
    }
    VK::Attachment::User user;
    json info = jres.at("response").get<json>();
    if (info.begin() == info.end()) {
        return "";
    }
    info = info.begin().value();
    user.parse(info);
    return user.first_name + " " + user.last_name;
}
