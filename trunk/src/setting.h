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
 
#ifndef SETTING_H
#define SETTING_H

#include <ZKbMainWidget.h> 
#include <ZFormContainer.h> 
#include <ZSettingItem.h> 
#include <ZListBox.h> 
#include <SETUP_SET_Manager.h>
#include "main.h"

class Setting : public ZKbMainWidget{
	Q_OBJECT
	
private:
	ZFormContainer *form;
	ZSettingItem *item;
	ZListBox *listbox;
	QString GPRS_Profile;
	SETUP_SET_Manager *cfg;
	
public:
	Setting(QWidget *parent);
	virtual ~Setting();
	void CreateUI();
	void showDlg();

private slots:	
	void slotQuit();
	void slot_changeOption(int);
	
};
	
extern Setting *setting;

#endif
