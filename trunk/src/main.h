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
 
#ifndef MAIN_H
#define MAIN_H

#include <qsocket.h>
#include <qtimer.h>
#include <qpixmap.h>

#include <ZKbMainWidget.h> 
#include <ZSingleSelectDlg.h>
#include <ZAppInfoArea.h>
#include <ZFormContainer.h>
#include <ZSoftKey.h>
#include <ZMultiLineEdit.h>
#include <ZNoticeDlg.h>
#include <ZProgressDlg.h>
#include <ZPressButton.h>
#include <SETUP_SET_Manager.h>

#include "zConnection.h"

class MainWidget : public ZKbMainWidget{
	Q_OBJECT
	
private:
	ZSingleSelectDlg *LangSelect;
	ZOptionsMenu *menu,*keymenu;	
	ZSoftKey *softKey;
	ZAppInfoArea * zapp;
	ZFormContainer *form;
	ZLineEdit *LineEdit1, *LineEdit2;
	ZMultiLineEdit *MultiLine;
	ZNoticeDlg *NoticeDlg;
	Connection *netconnection;
	ZProgressDlg *pProgressDialog;
	
	QSocket socket;
	QTimer *timer;
	bool nconnect;
	SETUP_SET_Manager *cfg;
	QPixmap pm;
public:
	MainWidget(QWidget *parent);
	virtual ~MainWidget();
	void CreateUI();
	void buttonUpdate();
	QString FullLangNameFromShort(QString name);
	QString lang_src, lang_dst;
	int lang;
	QString GPRS_Profile, GPRS_Profile_Old;
	ZPressButton *button1, *button2;

protected:
	bool qwsEvent( QWSEvent * );
	
private slots:	
	void slotQuit();
	void slotConnect();
	void slotConnectInternet();
	void next();
	void bytesWritten();
	void hostFound();
	void error(int code);
	void slotShowSrcLng();
	void timerEvent();
	void slotSetting();
	void about();
};
	
extern MainWidget *wgMain;

typedef struct __tGoogleLang {
    QString name;
    char code[16];
} tGoogleLang;

const tGoogleLang gGoogleLang[] = {
	QString::fromUtf8("Азербайджанский"), "az",
	QString::fromUtf8("Албанский" ), "sq",
	QString::fromUtf8("Английский"), "en",
	QString::fromUtf8("Арабский"), "ar",
	QString::fromUtf8("Армянский"), "hy",
	QString::fromUtf8("Африкаанс"), "af",
	QString::fromUtf8("Баскский"), "eu",
	QString::fromUtf8("Белорусский"), "be",
	QString::fromUtf8("Бенгальский"), "bn",
	QString::fromUtf8("Болгарский"), "bg",
	QString::fromUtf8("Валлийский"), "cy",
	QString::fromUtf8("Венгерский"), "hu",
	QString::fromUtf8("Вьетнамский"), "vi",
	QString::fromUtf8("Галисийский"), "gl",
	QString::fromUtf8("Голландский"), "nl",
	QString::fromUtf8("Греческий"), "el",
	QString::fromUtf8("Грузинский"), "ka",
	QString::fromUtf8("Гуджарати"), "gu",
	QString::fromUtf8("Датский"), "da",
	QString::fromUtf8("Иврит"), "iw",
	QString::fromUtf8("Идиш"), "yi",
	QString::fromUtf8("Индонезийский"), "id",
	QString::fromUtf8("Ирландский"), "ga",
	QString::fromUtf8("Исландский"), "is", // 0
	QString::fromUtf8("Испанский"), "es",
	QString::fromUtf8("Итальянский"), "it",
	QString::fromUtf8("Каннада"), "kn", // 0
	QString::fromUtf8("Каталанский"), "ca",
	QString::fromUtf8("Китайский"), "zh-CN",
	QString::fromUtf8("Корейский"), "ko", // 0
	QString::fromUtf8("Креольский (Гаити)"), "ht", // 0
	QString::fromUtf8("Латынь"), "la", // 0
	QString::fromUtf8("Латышский"), "lv",
	QString::fromUtf8("Литовский"), "lt",
	QString::fromUtf8("Македонский"), "mk",
	QString::fromUtf8("Малайский"), "ms", // 0
	QString::fromUtf8("Мальтийский"), "mt",
	QString::fromUtf8("Немецкий"), "de",
	QString::fromUtf8("Норвежский"), "no",
	QString::fromUtf8("Персидский"), "fa", // 0
	QString::fromUtf8("Польский"), "pl",
	QString::fromUtf8("Португальский"), "pt",
	QString::fromUtf8("Румынский"), "ro",
	QString::fromUtf8("Русский"), "ru",
	QString::fromUtf8("Сербский"), "sr",
	QString::fromUtf8("Словацкий"), "sk",
	QString::fromUtf8("Словенский"), "sl",
	QString::fromUtf8("Суахили"), "sw", // 0
	QString::fromUtf8("Тагальский"), "tl", // 0
	QString::fromUtf8("Тайский"), "th", 
	QString::fromUtf8("Тамильский"), "ta",
	QString::fromUtf8("Телугу"), "te",
	QString::fromUtf8("Турецкий"), "tr",
	QString::fromUtf8("Украинский"), "uk",
	QString::fromUtf8("Урду"), "ur",
	QString::fromUtf8("Финский"), "fi",
	QString::fromUtf8("Французский"), "fr",
	QString::fromUtf8("Хинди"), "hi",
	QString::fromUtf8("Хорватский"), "hr",
	QString::fromUtf8("Чешский"), "cs",
	QString::fromUtf8("Шведский"), "sv",
	QString::fromUtf8("Эстонский"), "et",
	QString::fromUtf8("Японский"), "ja"
};

const int gGoogleLangCount = sizeof( gGoogleLang ) / sizeof( tGoogleLang );

#endif
