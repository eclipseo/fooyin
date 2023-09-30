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

#include "filteritem.h"

#include <core/models/trackfwd.h>

#include <utils/tablemodel.h>

namespace Fy::Filters {
class FilterField;

class FilterModel : public Utils::TableModel<FilterItem>
{
public:
    explicit FilterModel(FilterField* field, QObject* parent = nullptr);
    ~FilterModel() override;

    [[nodiscard]] Qt::SortOrder sortOrder() const;

    void setField(FilterField* field);
    void setSortOrder(Qt::SortOrder order);
    void setRowHeight(int height);
    void setFont(const QFont& font);
    void setColour(const QColor& colour);

    [[nodiscard]] QVariant data(const QModelIndex& index, int role) const override;
    [[nodiscard]] Qt::ItemFlags flags(const QModelIndex& index) const override;
    [[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

    void sortFilter();
    //    [[nodiscard]] QModelIndexList match(const QModelIndex& start, int role, const QVariant& value, int hits,
    //                                        Qt::MatchFlags flags) const override;
    void reload(const Core::TrackList& tracks);

    void addTracks(const Core::TrackList& tracks);
    void updateTracks(const Core::TrackList& tracks);
    void removeTracks(const Core::TrackList& tracks);

private:
    struct Private;
    std::unique_ptr<Private> p;
};
} // namespace Fy::Filters
