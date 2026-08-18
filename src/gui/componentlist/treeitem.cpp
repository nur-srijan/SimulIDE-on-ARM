/***************************************************************************
 *   Copyright (C) 2024 by Santiago González                               *
 *                                                                         *
 ***( see copyright.txt file at root folder )*******************************/

#include "treeitem.h"
#include "mainwindow.h"

TreeItem::TreeItem( TreeItem* parent, QString name, QString nameTr, QString compType, treItemType_t itemType,
                    const QIcon& icon, bool custom )
    : QTreeWidgetItem() {
    m_parent = parent;
    m_name = name;
    m_nameTr = nameTr;
    m_compType = compType;
    m_isCustom = custom;
    m_itemType = type_NONE;

    m_shortcut = "";
    m_expanded = false;
    //m_hidden   = false;

    setIcon( 0, icon );
    setItemType( itemType );
}
TreeItem::~TreeItem() { }

void TreeItem::setItemType( treItemType_t itemType ) {
    m_itemType = itemType;

    float scale = MainWindow::self()->fontScale();
    QFont font;
    font.setFamily( MainWindow::self()->defaultFontName() );
    font.setBold( true );

    if ( itemType == component ) {
        setFlags( QFlag( Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled ) );

        if ( icon( 0 ).isNull() )
            setSizeHint( 0, QSize( 100, 14 * scale ) );

        font.setPixelSize( 11 * scale );
    } else { // Is Category
        setChildIndicatorPolicy( TreeItem::ShowIndicator );
        setFlags( QFlag( Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsDragEnabled ) );

        if ( itemType == categ_MAIN ) {
            setSizeHint( 0, QSize( 100, 30 * scale ) );
            font.setPixelSize( 13 * scale );
        } else if ( itemType == categ_CHILD ) {
            if ( icon( 0 ).isNull() )
                setSizeHint( 0, QSize( 100, 16 * scale ) );
            else
                setSizeHint( 0, QSize( 100, 20 * scale ) );
            font.setPixelSize( 12 * scale );
        }
    }
    setFont( 0, font );
    updateColors();
}

void TreeItem::updateColors() {
    bool dark = MainWindow::self() ? MainWindow::self()->isDarkMode() : false;

    if ( m_itemType == component ) {
        if ( dark ) {
            setForeground( 0, m_isCustom ? QColor( 160, 195, 235 ) : QColor( 220, 220, 220 ) );
        } else {
            setForeground( 0, m_isCustom ? QColor( 80, 90, 110 ) : QColor( 100, 90, 60 ) );
        }
    } else { // Is Category
        if ( m_itemType == categ_MAIN ) {
            if ( dark ) {
                if ( m_isCustom ) {
                    setForeground( 0, QColor( 210, 230, 250 ) );
                    setBackground( 0, QBrush( QColor( 38, 52, 68 ) ) );
                } else {
                    setForeground( 0, QColor( 220, 240, 232 ) );
                    setBackground( 0, QBrush( QColor( 42, 62, 56 ) ) );
                }
            } else {
                if ( m_isCustom ) {
                    setForeground( 0, QColor( 50, 60, 80 ) );
                    setBackground( 0, QBrush( QColor( 220, 235, 240 ) ) );
                } else {
                    setForeground( 0, QColor( 75, 70, 10 ) );
                    setBackground( 0, QBrush( QColor( 220, 240, 235 ) ) );
                }
            }
        } else if ( m_itemType == categ_CHILD ) {
            if ( dark ) {
                if ( m_isCustom ) {
                    setForeground( 0, QColor( 195, 218, 238 ) );
                    setBackground( 0, QBrush( QColor( 32, 44, 58 ) ) );
                } else {
                    setForeground( 0, QColor( 205, 228, 220 ) );
                    setBackground( 0, QBrush( QColor( 34, 50, 45 ) ) );
                }
            } else {
                if ( m_isCustom ) {
                    setForeground( 0, QColor( 70, 80, 100 ) );
                    setBackground( 0, QBrush( QColor( 230, 245, 250 ) ) );
                } else {
                    setForeground( 0, QColor( 90, 80, 50 ) );
                    setBackground( 0, QBrush( QColor( 230, 250, 245 ) ) );
                }
            }
        }
    }
}

void TreeItem::setItemExpanded( bool e ) {
    m_expanded = e;
    setExpanded( e );
}

//void TreeItem::setItemHidden( bool h )
//{
//    m_hidden = h;
//    setHidden( h );
//}

QString TreeItem::toString( QString indent ) {
    QString catStr;

    if ( m_itemType > component )
        catStr = indent + "<category ";
    else
        catStr = indent + "<component ";
    catStr += "name=\"" + m_name + "\" ";
    //catStr += "nametr=\""  +m_nameTr+"\" ";
    //catStr += "comptype=\""+m_compType+"\" ";
    catStr += "hidden=\"" + QString::number( isHidden() ? 1 : 0 ) + "\" ";

    if ( m_itemType > component )
        catStr += "expanded=\"" + QString::number( isExpanded() ? 1 : 0 ) + "\" > \n";
    else
        catStr += "shortcut=\"" + m_shortcut + "\" />\n";

    for ( int i = 0; i < childCount(); ++i ) {
        TreeItem* childItem = (TreeItem*) child( i );
        catStr += childItem->toString( indent + "  " );
    }
    if ( m_itemType > component )
        catStr += indent + "</category>\n";

    return catStr;
}
