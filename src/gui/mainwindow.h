/*
 * Fooyin
 * Copyright 2022-2023, Luke Taylor <LukeT1@proton.me>
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

#pragma once

#include <QMainWindow>

namespace Fy {

namespace Utils {
class ActionManager;
class SettingsManager;
} // namespace Utils

namespace Core {
namespace Library {
class LibraryManager;
}

namespace Player {
class PlayerManager;
}
} // namespace Core

namespace Gui {
class LayoutProvider;
class QuickSetupDialog;
class MainMenuBar;
class FileMenu;
class EditMenu;
class ViewMenu;
class PlaybackMenu;
class LibraryMenu;
class HelpMenu;

namespace Widgets {
class EditableLayout;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Utils::ActionManager* actionManager, Core::Player::PlayerManager* playerManager,
                        Core::Library::LibraryManager* libraryManager, Utils::SettingsManager* settings,
                        LayoutProvider* layoutProvider, Widgets::EditableLayout* editableLayout,
                        QWidget* parent = nullptr);

    void shutdown();

    void setupUi();
    void setupMenu();

signals:
    void closing();

protected:
    void closeEvent(QCloseEvent* event) override;

    void enableLayoutEditing(bool enable);

private:
    void registerLayouts() const;

    Utils::ActionManager* m_actionManager;
    Core::Player::PlayerManager* m_playerManager;
    Core::Library::LibraryManager* m_libraryManager;
    Utils::SettingsManager* m_settings;
    Widgets::EditableLayout* m_editableLayout;

    MainMenuBar* m_mainMenu;
    FileMenu* m_fileMenu;
    EditMenu* m_editMenu;
    ViewMenu* m_viewMenu;
    PlaybackMenu* m_playbackMenu;
    LibraryMenu* m_libraryMenu;
    HelpMenu* m_helpMenu;

    LayoutProvider* m_layoutProvider;
    QuickSetupDialog* m_quickSetupDialog;
};
} // namespace Gui
} // namespace Fy
