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
#include "lngselectdlg.h"

LngView *lngView = 0;

LngView::LngView(QWidget *parent)//:ZKbMainWidget(ZHeader::MAINDISPLAY_HEADER,parent,0,0)
{
	CreateUI();
	showDlg();
}

LngView::~LngView() 
{

}

void LngView::CreateUI()
{
	setMainWidgetTitle("Выберите язык:");
	
	lngview = new ZIconView(this, NULL, 0, 20, ZSkinService::clsZIconView2);
	
	//lngview->setShowLabel(false);
	/*
	lngview->setItemSize(QSize(50, 50));
	lngview->setItemSizeFixed(true);
	lngview->setLayout(2,8);
	*/
	for ( uint i = 0; i < gGoogleLangCount; i++ ) 
	{
		ZIconViewItem *item = new ZIconViewItem(lngview);
		//item->setType(ZIconViewItem::MARK_CHECKBOX);
		QPixmap pixmap = QPixmap(QString("img/flag/%1.png").arg(gGoogleLang[i].code));
		item->setPixmap(pixmap, false, false);
		item->setText(gGoogleLang[i].name);
		//item->setPixmapRect(QRect(0,0,50,50));
	}
	
	connect(lngview, SIGNAL(selectionChanged(ZIconViewItem*)), this, SLOT(slotChanged(ZIconViewItem*)));
	connect(lngview, SIGNAL(returnPressed(ZIconViewItem*)), this, SLOT(slotSelect(ZIconViewItem*)));
	
	ZSoftKey *softKey = new ZSoftKey(NULL , this , this);
	softKey->setText(ZSoftKey::RIGHT, tr("Back"), (ZSoftKey::TEXT_PRIORITY)0);
	softKey->setClickedSlot(ZSoftKey::RIGHT, this, SLOT(slotQuit()));
	
	setContentWidget(lngview);
	setSoftKey(softKey);
}

void LngView::slotChanged(ZIconViewItem * item)
{
	//int i = item->index();
	//setMainWidgetTitle(QString("Selected Lang: %1").arg(gGoogleLang[i].name));
}

void LngView::slotSelect(ZIconViewItem * item)
{
	int i = item->index();
	if (!wgMain->lang) {
		wgMain->lang_src = gGoogleLang[i].code;
		}
		else
		{
		wgMain->lang_dst = gGoogleLang[i].code;
		}
	wgMain->buttonUpdate();
	slotQuit();
}

void LngView::slotQuit()
{
	this->hide();
}

void LngView::showDlg()
{
	for (int i = 0; i < lngview->count();++i)
	{
		if (wgMain->lang == 0)
		{
			if (lngview->item(i)->text() == wgMain->FullLangNameFromShort(wgMain->lang_src)) lngview->setCurrentItem(lngview->item(i));
		}
		else 
		{	
			if (lngview->item(i)->text() == wgMain->FullLangNameFromShort(wgMain->lang_dst)) lngview->setCurrentItem(lngview->item(i));
		}
	}
	this->show();
}
