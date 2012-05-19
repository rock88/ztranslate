/*
 *  zTranslate by rock88 (c)
 *  e-mail: rock88a@gmail.com
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
 
#include <stdio.h>

#include <ZNoticeDlg.h>
#include <ZOptionsMenu.h>
#include <ZMessageDlg.h>
#include <ZApplication.h>

#include "main.h"
#include "helper.h"
#include "lngselectdlg.h"
#include "setting.h"

#define ALT_TRANSLATE 0

int dot_array[1024];
int dot_count = 0;
MainWidget *wgMain = 0;
int getProgress = 0;

MainWidget::MainWidget(QWidget *parent):ZKbMainWidget(ZHeader::MAINDISPLAY_HEADER,parent,0,0),nconnect(false)
{
	cfg = new SETUP_SET_Manager(QString("setting.ini"));
	
	cfg->getSettingInfo(QString("Main"), QString("lang_src"), lang_src, "en");
	cfg->getSettingInfo(QString("Main"), QString("lang_dst"), lang_dst, "ru");
	
	cfg->getSettingInfo(QString("Main"), QString("GPRS_Profile"), GPRS_Profile, "0");
	GPRS_Profile_Old = GPRS_Profile;
	//lang_src = "en";
	//lang_dst = "ru";
	lang = 0;
	
	memset(dot_array, 0x00, 1024);
	timer = new QTimer( this );
	connect(timer, SIGNAL(timeout()), SLOT(timerEvent()));
	
	CreateUI();
}

MainWidget::~MainWidget() 
{

}

void MainWidget::CreateUI()
{
	qDebug("MainWidget::CreateUI()");
	
	setMainWidgetTitle("zTranslate / 1.0");
	
	//soft keys
	softKey = new ZSoftKey(NULL , this , this);
	softKey->setText(ZSoftKey::LEFT, "Menu", (ZSoftKey::TEXT_PRIORITY)0);
	softKey->setTextForOptMenuHide("Menu");
	QRect rect = ZGlobal::getContentR();

	
	menu = new ZOptionsMenu(rect, this, 0, 0, (ZSkinService::WidgetClsID)55);
	menu->setItemSpacing(10);
	
	pm = QPixmap(QString("img/icon/translate.png"));
	menu->insertItem(tr(" Перевести"),&pm,this,SLOT(slotConnectInternet()));
	
	pm = QPixmap(QString("img/icon/options.png"));
	menu->insertItem(tr(" Настройки"),&pm,this,SLOT(slotSetting()));
	
	pm = QPixmap(QString("img/icon/about.png"));
	menu->insertItem(tr(" О программе..."),&pm,this,SLOT(about()));
	
	pm = QPixmap(QString("img/icon/exit.png"));
	menu->insertItem(tr(" Выход"),&pm,this,SLOT(slotQuit()));
	
	softKey->setText(ZSoftKey::RIGHT, tr("Выход"), (ZSoftKey::TEXT_PRIORITY)0);
	softKey->setClickedSlot(ZSoftKey::RIGHT, this, SLOT(slotQuit()));
	softKey->setOptMenu(ZSoftKey::LEFT, menu);

	zapp = new ZAppInfoArea(this,"app info",  0, ZSkinService::clsZAppInfoArea);	
    zapp->insertText(ZAppInfoArea::ACTIVITY, 0, "", NULL);

	form = new ZFormContainer(this, 0, ZSkinService::clsZFormContainer);
	

	
	LineEdit1 = new ZLineEdit(this);
	form->addChild(LineEdit1);

	button1 = new ZPressButton(this);
	connect(button1,SIGNAL(pressed()),SLOT(slotShowSrcLng()));
	buttonUpdate();
	form->addChild(button1);

	MultiLine = new ZMultiLineEdit(this);
	form->addChild(MultiLine);
	
	LineEdit1->setText("");
	MultiLine->setText("");
	
	setContentWidget(form);
	setTitleBarWidget(zapp);
	setSoftKey(softKey);	
	
	show();
}

void MainWidget::slotShowSrcLng()
{
	QStringList qlist;
	LangSelect = new ZSingleSelectDlg(this);
	LangSelect->setTitle("Выбор языков:");
	qlist.append(QString("С: %1").arg(FullLangNameFromShort(lang_src)));
	qlist.append(QString("На: %1").arg(FullLangNameFromShort(lang_dst)));
	qlist.append("Поменять местами <->");
	LangSelect->addItemsList(qlist);
	if (LangSelect->exec() == 1)
	{
		if (LangSelect->getCheckedItemIndex() <= 1)
		{
			lang = LangSelect->getCheckedItemIndex();
			//printf("[] lang = %i\n",lang);
			if (!lngView) lngView = new LngView(this);
				else lngView->showDlg();
		}
		else
		{
			QString buf = lang_src;
			lang_src = lang_dst;
			lang_dst = buf;
		}
	}
	delete LangSelect;
	
	buttonUpdate();
}

void MainWidget::buttonUpdate()
{
	button1->setText(QString(" %1 -> %2 ").arg(FullLangNameFromShort(lang_src)).arg(FullLangNameFromShort(lang_dst)));
}

QString MainWidget::FullLangNameFromShort(QString name)
{
	for ( uint i = 0; i < gGoogleLangCount; i++ )
	{
		if (QString(gGoogleLang[i].code) == name) return QString(gGoogleLang[i].name);
	}
	return "NULL";
}

void MainWidget::slotQuit()
{
	cfg->setSettingInfo(QString("Main"), QString("lang_src"), lang_src);
	cfg->setSettingInfo(QString("Main"), QString("lang_dst"), lang_dst);
	cfg->flush();
	delete cfg;
	
	((ZApplication *)qApp)->quit();
}

void MainWidget::slotConnect()
{
#if ALT_TRANSLATE
	socket.connectToHost("ajax.googleapis.com", 80);
#else
	socket.connectToHost("www.google.com", 80);
#endif
}


void MainWidget::error(int code) 
{ 
	switch (code) { 
		case QSocket::ErrConnectionRefused: 
			printf("Error: Connection refused\n"); 
			break; 
		case QSocket::ErrHostNotFound: 
			printf("Error: Server not found\n"); 
			break; 
		case QSocket::ErrSocketRead: 
		default: 
			printf("Error: Data transfer failed\n"); 
	}
}

void MainWidget::hostFound()
{
	//printf("hostFound()\n");
}

void MainWidget::next(){
	//printf("connected()\n");
	
	pProgressDialog = new ZProgressDlg("", "Пожалуйста, подождите...", 140, 100, this);
	ZSoftKey* zsoft = pProgressDialog->getSoftKey();

	zsoft->disableClickedSlot(ZSoftKey::LEFT);
	zsoft->disableClickedSlot(ZSoftKey::RIGHT);

	zsoft->setText(ZSoftKey::LEFT, "", ZSoftKey::ZERO);
	zsoft->setText(ZSoftKey::RIGHT, "", ZSoftKey::ZERO);
	
	getProgress = 0;
	pProgressDialog->setProgress(getProgress);
	pProgressDialog->raise();
	pProgressDialog->show();

	if (!timer->isActive()) timer->start(500, FALSE);
	
	QString text1 = LineEdit1->text();
	
	text1 = decodeChar(text1); // TODO: to do something about it

	QString text = QString::fromUtf8(toPercentEncoding(text1));
	printf("Text=%s\n",text.latin1());
#if ALT_TRANSLATE
	QString url2 = QString(	"GET "
							"/ajax/services/language/translate?v=2.0&;hl=ru&q="
							+ QString(text)
							+ "&langpair="
							+ QString(lang_src)
							+ "|"
							+ QString(lang_dst) +
							" HTTP/1.1\r\n"
							"User-Agent: " "Mozilla/5.0" "\r\n"
							"Host: " "ajax.googleapis.com" "\r\n"
							"Connection: Close\r\n\r\n"							
							);
#else
	QString url2 = QString(	"GET "
							"/translate_a/t?client=t&sl="
							+ QString(lang_src)
							+ 	"&tl="
							+ QString(lang_dst) +
							+ "&text="
							+ QString(text) +
							" HTTP/1.1\r\n"
							"User-Agent: " "Mozilla/5.0" "\r\n"
							"Host: " "www.google.com " "\r\n"
							"Connection: Close\r\n\r\n"
							);
#endif						
	socket.writeBlock(url2, url2.length());


}

void MainWidget::bytesWritten(){
	if (timer->isActive()) timer->stop();
	pProgressDialog->hide();
	delete pProgressDialog;
	printf("bytesWritten()\n");
	int i = socket.bytesAvailable();
	char *c = (char*)malloc(i*4);
	printf("bytesAvailable=%i\n",socket.bytesAvailable());
	socket.readBlock( c, i );
	QString answer = QString(QString::fromUtf8(c));
	//printf("Answer = \n[Start]\n%s\n[End]\n",c);
#if ALT_TRANSLATE
	QString s1 = answer;
#else
	int i1 = answer.find("[[[\"",0,true);
	int i2 = answer.find("\",\"",0,true);

	QString s1 = answer.mid(i1+4,i2-(i1+4));
#endif

	s1 = encodeChar(s1); // TODO: and this
	
	MultiLine->setText(s1);
	answer = QString();
	s1 = QString();
	free(c);
	socket.close();
}

void MainWidget::slotConnectInternet()
{	
	if (LineEdit1->text() == QString(""))
	{
		NoticeDlg = new ZNoticeDlg(ZNoticeDlg::Type(0),"Введите текст для перевода!","",this);
		NoticeDlg->setTitle("Ошибка:");
		QPixmap pixmap = QPixmap(QString("img/notice/error.png"));
		NoticeDlg->setTitleIcon(pixmap);
		NoticeDlg->exec();
		delete NoticeDlg;
		return;
	}
/*	
	if (GPRS_Profile_Old != GPRS_Profile)
	{
		nconnect = false;
		GPRS_Profile_Old = GPRS_Profile;
		if (netconnection)
		{
			netconnection->Disconnect();
			delete netconnection;
		}
	}
*/	
	if (!nconnect)
	{
		if (GPRS_Profile == "0")
		{
			QStringList qslList = NAPIManager::getProfiles();
			ZSingleSelectDlg *dlg = new ZSingleSelectDlg("Выберите профиль:", "", this);
			dlg->addItemsList(qslList);
			if ( dlg->exec() == QDialog::Accepted )
			{
				int n=dlg->getCheckedItemIndex();
				if (n>=0){
					netconnection = new Connection(this);
					connect(netconnection, SIGNAL(connected()), SLOT(slotConnect()));
					
					connect(&socket, SIGNAL(connected()), this, SLOT(next()));
					connect(&socket, SIGNAL(error(int)), this, SLOT(error(int))); 
					connect(&socket, SIGNAL(readyRead()), this, SLOT(bytesWritten()));	
		
					netconnection->Connect(*(qslList.at(n)));
					nconnect = true;
					}
			}
			delete dlg;
		}
		else
		{
			netconnection = new Connection(this);
			connect(netconnection, SIGNAL(connected()), SLOT(slotConnect()));
			
			connect(&socket, SIGNAL(connected()), this, SLOT(next()));
			connect(&socket, SIGNAL(error(int)), this, SLOT(error(int))); 
			connect(&socket, SIGNAL(readyRead()), this, SLOT(bytesWritten()));	

			netconnection->Connect(GPRS_Profile);
			nconnect = true;
		}
	} else slotConnect();
}

bool MainWidget::qwsEvent( QWSEvent *ev )
{
	QWSKeyEvent *kv;
	kv = (QWSKeyEvent *)ev;
	
	if (kv->simpleData.is_press == 1)
	{
		if (LineEdit1->hasFocus()) 
			if (kv->simpleData.keycode == KEYCODE_SEND)
			{
				slotConnectInternet();
				/*
				QString text1 = LineEdit1->text();

				decodeChar(text1);
				encodeChar(text1);
					
				MultiLine->setText(text1);
				*/
			}
	}
	
	return QWidget::qwsEvent(ev);
}

void MainWidget::timerEvent()
{
	getProgress+=10;
	pProgressDialog->setProgress(getProgress);
}

void MainWidget::slotSetting()
{
	if (!setting) setting = new Setting(this);
		else setting->showDlg();
}

void MainWidget::about()
{
	ZMessageDlg *dlg = new ZMessageDlg(	"О программе...",""
										"zTranslate / 1.0 - "
										//"\n\n"
										"Программа-переводчик, использующая для перевода сервис Google Translate.\n"
										"Copyrigth ©: rock88, 2011\n"
										"e-mail: rock88a@gmail.com\n"
										,(ZMessageDlg::MessageDlgType)0);
	ZSoftKey* zsoft = dlg->getSoftKey();
	zsoft->disableClickedSlot(ZSoftKey::LEFT);
	zsoft->setText(ZSoftKey::LEFT, "", ZSoftKey::ZERO);
	zsoft->setText(ZSoftKey::RIGHT, "Back", ZSoftKey::ZERO);
	dlg->exec();
	delete dlg;
}


int main(int argc, char **argv){
	printf("%s\n",QString("Start...").latin1());
    ZApplication app(argc, argv);
	wgMain = new MainWidget(0);
    app.setMainWidget( wgMain );
    wgMain->show();
    return app.exec();
	}

	
