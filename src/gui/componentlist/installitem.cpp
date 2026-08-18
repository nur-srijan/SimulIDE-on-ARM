/***************************************************************************
 *   Copyright (C) 2025 by santiago González                               *
 *                                                                         *
 ***( see copyright.txt file at root folder )*******************************/

#include <QDebug>
#include <QPushButton>
#include <unistd.h>

#include "installer.h"
#include "installinfo.h"
#include "installitem.h"
#include "mainwindow.h"

InstallItem::InstallItem( Installer* parent, QString item ) : QWidget( (QWidget*) parent ) {
    m_Installer = parent;
    m_infoWidget = nullptr;

    setupUi( this );
    setItem( item );
}

bool InstallItem::shouldUpdate( int64_t v ) {
    if ( m_version == m_versionNext ) {
        setButtonState( bUninstall );
        return false;
    }
    if ( v == 0 )
        setButtonState( bInstall );
    else
        setButtonState( bUpdate );
    return true;
}

void InstallItem::setItem( QString itemStr ) {
    m_itemStr = itemStr;

    float scale = MainWindow::self()->fontScale();
    QFont font;
    font.setFamily( MainWindow::self()->defaultFontName() );

    font.setPixelSize( 11 * scale );
    textEdit->setFont( font );

    infoButton->setIcon( QIcon( ":/help.svg" ) );
    infoButton->setToolTip( tr( "Information" ) );

    updtButton->setIcon( QIcon( ":/reset.svg" ) );
    updtButton->setToolTip( tr( "Update" ) );

    QStringList set = itemStr.split( "; " );

    m_name = set.at( 0 );

    QString header = "#### ";

    if ( set.size() < 4 ) {
        installButton->hide();
        infoButton->hide();
        updtButton->hide();
        gridLayout->setContentsMargins( 2, 2, 2, 2 );

        font.setPixelSize( 13 * scale );
        setNameEdit->setFont( font );

        textEdit->setVisible( false );

        header = "### ";
    } else {
        m_description = set.at( 1 );
        m_file = set.at( 2 );
        m_versionNext = set.at( 3 ).toLongLong();
        m_version = 0;

        if ( set.size() > 4 )
            m_depends = set.at( 4 );
        if ( set.size() > 5 )
            m_author = set.at( 5 );

        font.setPixelSize( 12 * scale );
        setNameEdit->setFont( font );

        textEdit->setMarkdown( m_description );
    }

    setNameEdit->setMarkdown( header + m_name );

    updateColors();

    QObject::connect( installButton, &QPushButton::clicked, [=]() { installClicked(); } );
    QObject::connect( infoButton, &QPushButton::clicked, [=]() { infoClicked(); } );
}

void InstallItem::infoClicked() {
    if ( !m_infoWidget ) {
        QStringList items = m_Installer->getGroupItems( m_name );
        m_infoWidget = new InstallInfo( this, m_description, m_author, items );
        m_infoWidget->setWindowTitle( m_name );
    }
    m_infoWidget->show();
}

void InstallItem::installClicked() {
    switch ( m_buttonState ) {
    case bInstall:
    case bUpdate:
        m_Installer->installItem( m_name );
        m_version = m_versionNext;
        setButtonState( bUninstall );
        break;
    case bUninstall:
        m_Installer->unInstallItem( m_name );
        setButtonState( bInstall );
        break;
    }
}

void InstallItem::updated() {
    setButtonState( bUninstall );
}

void InstallItem::setButtonState( buttonState_t state ) {
    m_buttonState = state;
    updateColors();
}

void InstallItem::updateColors() {
    bool dark = MainWindow::self() ? MainWindow::self()->isDarkMode() : false;

    if ( m_file.isEmpty() ) {
        QPalette p = setNameEdit->palette();
        if ( dark ) {
            p.setColor( QPalette::Base, QColor( 42, 62, 56 ) );
            p.setColor( QPalette::Window, QColor( 42, 62, 56 ) );
            p.setColor( QPalette::Text, QColor( 220, 240, 232 ) );
        } else {
            p.setColor( QPalette::Base, QColor( 220, 240, 235 ) );
            p.setColor( QPalette::Window, QColor( 220, 240, 235 ) );
            p.setColor( QPalette::Text, QColor( 50, 70, 60 ) );
        }
        this->setAutoFillBackground( true );
        this->setPalette( p );
        setNameEdit->setPalette( p );
        return;
    }

    bool update = ( m_buttonState == bUpdate );
    updtButton->setEnabled( update );

    QPalette pb = updtButton->palette();
    if ( update )
        pb.setColor( QPalette::Button, dark ? QColor( 140, 130, 40 ) : QColor( "#DDDD55" ) );
    else
        pb.setColor( QPalette::Button, dark ? QColor( 50, 55, 62 ) : QColor( "#EEEEEE" ) );
    updtButton->setPalette( pb );

    pb = infoButton->palette();
    pb.setColor( QPalette::Button, dark ? QColor( 55, 60, 70 ) : QColor( "#DDDDDD" ) );
    infoButton->setPalette( pb );

    pb = installButton->palette();
    bool installed = ( m_buttonState != bInstall );

    QColor bgColor;
    QColor nameTextColor;
    QColor descTextColor;

    if ( installed ) {
        installButton->setToolTip( tr( "Uninstall" ) );
        installButton->setIcon( QIcon( ":/remove.svg" ) );
        pb.setColor( QPalette::Button, dark ? QColor( 170, 55, 55 ) : QColor( "#FF8070" ) );

        if ( update ) {
            bgColor = dark ? QColor( 45, 48, 28 ) : QColor( "#CCFFCC" );
            nameTextColor = dark ? QColor( 245, 240, 200 ) : QColor( "#203010" );
            descTextColor = dark ? QColor( 195, 190, 160 ) : QColor( "#506040" );
        } else {
            bgColor = dark ? QColor( 28, 46, 36 ) : QColor( "#EAFFEA" );
            nameTextColor = dark ? QColor( 220, 245, 228 ) : QColor( "#103015" );
            descTextColor = dark ? QColor( 170, 195, 178 ) : QColor( "#406045" );
        }
    } else {
        installButton->setToolTip( tr( "Install" ) );
        installButton->setIcon( QIcon( ":/load.svg" ) );
        pb.setColor( QPalette::Button, dark ? QColor( 40, 130, 55 ) : QColor( "#30DD40" ) );

        bgColor = dark ? QColor( 34, 37, 44 ) : QColor( "#F8F8FF" );
        nameTextColor = dark ? QColor( 225, 225, 225 ) : QColor( "#222222" );
        descTextColor = dark ? QColor( 165, 170, 180 ) : QColor( "#666666" );
    }
    installButton->setPalette( pb );

    this->setAutoFillBackground( true );

    QPalette p = this->palette();
    p.setColor( QPalette::Base, bgColor );
    p.setColor( QPalette::Window, bgColor );
    this->setPalette( p );

    QPalette pName = setNameEdit->palette();
    pName.setColor( QPalette::Base, bgColor );
    pName.setColor( QPalette::Window, bgColor );
    pName.setColor( QPalette::Text, nameTextColor );
    setNameEdit->setPalette( pName );

    QPalette pDesc = textEdit->palette();
    pDesc.setColor( QPalette::Base, bgColor );
    pDesc.setColor( QPalette::Window, bgColor );
    pDesc.setColor( QPalette::Text, descTextColor );
    textEdit->setPalette( pDesc );
}

QString InstallItem::toString() {
    QString itemStr;
    itemStr += m_name + ";" + QString::number( m_version );

    //itemStr += m_name+"; ";
    //itemStr += m_description+"; ";
    //itemStr += m_file+"; ";
    //itemStr += QString::number( m_version )+"; ";
    //itemStr += m_depends+"; ";
    //itemStr += m_author+"; ";

    return itemStr;
}
