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
#include <ZSoftKey.h>
#include "setting.h"
#include "zConnection.h"

Setting *setting = 0;

Setting::Setting(QWidget *parent)//:ZKbMainWidget(ZHeader::MAINDISPLAY_HEADER,parent,0,0)
{
	GPRS_Profile = "0";
	showDlg();
}

Setting::~Setting() 
{

}

void Setting::CreateUI()
{
	setMainWidgetTitle("Настройки:");
	
	cfg = new SETUP_SET_Manager(QString("setting.ini"));
	cfg->getSettingInfo(QString("Main"), QString("GPRS_Profile"), GPRS_Profile, "0");
	
	listbox = new ZListBox(this, 0, ZSkinService::clsZFormContainer);
	item = new ZSettingItem(listbox);
	item->appendSubItem(0, "Профиль GPRS:");
	if (GPRS_Profile == "0") item->appendSubItem(0, "Спрашивать");
		else item->appendSubItem(0, GPRS_Profile);
	listbox->insertItem(item);
	
	
	connect( listbox, SIGNAL(selected(int)), this, SLOT(slot_changeOption(int)));
	
	ZSoftKey *softKey = new ZSoftKey(NULL , this , this);
	softKey->setText(ZSoftKey::RIGHT, tr("Back"), (ZSoftKey::TEXT_PRIORITY)0);
	softKey->setClickedSlot(ZSoftKey::RIGHT, this, SLOT(slotQuit()));
	
	setContentWidget(listbox);
	setSoftKey(softKey);
}

void Setting::slotQuit()
{
	cfg->setSettingInfo(QString("Main"), QString("GPRS_Profile"), GPRS_Profile);
	cfg->flush();
	
	delete cfg;
	delete item;
	delete listbox;
	
	this->hide();
}

void Setting::showDlg()
{
	CreateUI();
	this->show();
}

void Setting::slot_changeOption(int sel)
{
	if (sel == 0)
	{
		QStringList qslList = NAPIManager::getProfiles();
		QStringList qslList2;
		qslList2.append("Спрашивать");
		for (int i = 0; i < qslList.count(); ++i)
		{
			qslList2.append(qslList[i]);
		}
		
		ZSingleSelectDlg *dlg = new ZSingleSelectDlg("Выберите профиль*:", "", this);
		dlg->setInstructText("* требуется перезапуск");
		dlg->addItemsList(qslList2);
		if ( dlg->exec() == QDialog::Accepted )
		{
			int n=dlg->getCheckedItemIndex();
			if (n>=0)
			{
				if (n==0) GPRS_Profile = "0"; else GPRS_Profile = qslList2[n];
				ZSettingItem *item = (ZSettingItem *)listbox->item(sel);
				if (n==0) item->setSubItem(0, 1, "Спрашивать"); else item->setSubItem(0, 1, GPRS_Profile);
			}
		}
		delete dlg;
	}
}
