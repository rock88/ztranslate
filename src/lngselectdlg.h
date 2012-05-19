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
 
#ifndef LNGSELECTDLG_H
#define LNGSELECTDLG_H

#include <ZKbMainWidget.h> 
#include <ZIconView.h>
#include "main.h"

class LngView : public ZKbMainWidget{
	Q_OBJECT
	
private:
	ZIconView *lngview;
	QString *mlang;
	
public:
	LngView(QWidget *parent);
	virtual ~LngView();
	void CreateUI();
	void showDlg();
	
private slots:	
	void slotChanged(ZIconViewItem*);
	void slotSelect(ZIconViewItem*);
	void slotQuit();
};
	
extern LngView *lngView;

#endif
