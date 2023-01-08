/*
 * Fooyin
 * Copyright 2022, Luke Taylor <LukeT1@proton.me>
 *
 * Fooyin is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Fooyin is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Fooyin.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "controlwidget.h"

#include "core/library/models/track.h"
#include "core/player/playermanager.h"
#include "playercontrol.h"
#include "playlistcontrol.h"
#include "progresswidget.h"
#include "volumecontrol.h"

#include <QContextMenuEvent>
#include <QHBoxLayout>
#include <QMenu>
#include <pluginsystem/pluginmanager.h>

namespace Core::Widgets {
struct ControlWidget::Private
{
    QHBoxLayout* layout;
    PlayerControl* playControls;
    PlaylistControl* playlistControls;
    VolumeControl* volumeControls;
    ProgressWidget* progress;

    Player::PlayerManager* playerManager;

    explicit Private()
        : playerManager(PluginSystem::object<Player::PlayerManager>())
    { }
};

ControlWidget::ControlWidget(QWidget* parent)
    : FyWidget(parent)
    , p(std::make_unique<Private>())
{
    setObjectName("Control Bar");

    setupUi();
    setupConnections();
}

ControlWidget::~ControlWidget() = default;

void ControlWidget::setupUi()
{
    p->layout = new QHBoxLayout(this);

    //    setEnabled(false);

    p->playControls = new PlayerControl(this);
    p->playlistControls = new PlaylistControl(this);
    p->volumeControls = new VolumeControl(this);
    p->progress = new ProgressWidget(this);

    p->layout->addWidget(p->playControls, 0, Qt::AlignLeft | Qt::AlignVCenter);
    p->layout->addWidget(p->progress, 0, Qt::AlignVCenter);
    p->layout->addWidget(p->playlistControls, 0, Qt::AlignVCenter);
    p->layout->addWidget(p->volumeControls, 0, Qt::AlignVCenter);

    p->layout->setContentsMargins(0, 0, 0, 0);
    p->layout->setSpacing(15);
}

void ControlWidget::setupConnections()
{
    connect(p->playerManager, &Player::PlayerManager::currentTrackChanged, this, &ControlWidget::currentTrackChanged);
    connect(p->playerManager, &Player::PlayerManager::positionChanged, this, &ControlWidget::currentPositionChanged);
    connect(p->playerManager, &Player::PlayerManager::playStateChanged, p->playControls, &PlayerControl::stateChanged);
    connect(p->playerManager, &Player::PlayerManager::playStateChanged, p->progress, &ProgressWidget::stateChanged);
    connect(p->playControls, &PlayerControl::stopClicked, p->playerManager, &Player::PlayerManager::stop);
    connect(p->playControls, &PlayerControl::nextClicked, p->playerManager, &Player::PlayerManager::next);
    connect(p->playControls, &PlayerControl::prevClicked, p->playerManager, &Player::PlayerManager::previous);
    connect(p->playControls, &PlayerControl::stopClicked, p->progress, &ProgressWidget::reset);
    connect(p->playControls, &PlayerControl::pauseClicked, p->playerManager, &Player::PlayerManager::playPause);
    connect(p->volumeControls, &VolumeControl::volumeUp, p->playerManager, &Player::PlayerManager::volumeUp);
    connect(p->volumeControls, &VolumeControl::volumeDown, p->playerManager, &Player::PlayerManager::volumeDown);
    connect(p->volumeControls, &VolumeControl::volumeChanged, p->playerManager, &Player::PlayerManager::setVolume);
    connect(p->progress, &ProgressWidget::movedSlider, p->playerManager, &Player::PlayerManager::changePosition);
    connect(p->playlistControls, &PlaylistControl::repeatClicked, p->playerManager, &Player::PlayerManager::setRepeat);
    connect(p->playlistControls, &PlaylistControl::shuffleClicked, p->playerManager,
            &Player::PlayerManager::setShuffle);
    connect(p->playerManager, &Player::PlayerManager::playModeChanged, p->playlistControls,
            &PlaylistControl::playModeChanged);
}

QString ControlWidget::name() const
{
    return "Controls";
}

void ControlWidget::contextMenuEvent(QContextMenuEvent* event)
{
    Q_UNUSED(event)
}

void ControlWidget::currentTrackChanged(Track* track)
{
    //    if (!isEnabled())
    //    {
    //        setEnabled(true);
    //    }
    p->progress->changeTrack(track);
}

void ControlWidget::currentPositionChanged(qint64 ms)
{
    p->progress->setCurrentPosition(static_cast<int>(ms));
}
}; // namespace Core::Widgets
