#ifndef CSM_WOLRD_REGIONMAP_H
#define CSM_WOLRD_REGIONMAP_H

#include <map>
#include <string>
#include <vector>

#include <QAbstractTableModel>

#include "record.hpp"
#include "cell.hpp"

namespace CSMWorld
{
    class Data;

    /// \brief Model for the region map
    ///
    /// This class does not holds any record data (other than for the purpose of buffering).
    class RegionMap : public QAbstractTableModel
    {
            Q_OBJECT

        public:

            typedef std::pair<int, int> CellIndex;

        private:

            struct CellDescription
            {
                bool mDeleted;
                std::string mRegion;
                std::string mName;

                CellDescription();

                CellDescription (const Record<Cell>& cell);
            };

            Data& mData;
            std::map<CellIndex, CellDescription> mMap;
            CellIndex mMin; ///< inclusive
            CellIndex mMax; ///< exclusive
            std::map<std::string, unsigned int> mColours; ///< region ID, colour (RGBA)

            CellIndex getIndex (const QModelIndex& index) const;
            ///< Translates a Qt model index into a cell index (which can contain negative components)

            QModelIndex getIndex (const CellIndex& index) const;

            void buildRegions();

            void buildMap();

            void addCell (const CellIndex& index, const CellDescription& description);
            ///< May be called on a cell that is already in the map (in which case an update is
            // performed)

            void addCells (int start, int end);

            void removeCell (const CellIndex& index);
            ///< May be called on a cell that is not in the map (in which case the call is ignored)

            void addRegion (const std::string& region, unsigned int colour);
            ///< May be called on a region that is already listed (in which case an update is
            /// performed)
            ///
            /// \note This function does not update the region map.

            void removeRegion (const std::string& region);
            ///< May be called on a region that is not listed (in which case the call is ignored)
            ///
            /// \note This function does not update the region map.

            void updateRegions (const std::vector<std::string>& regions);
            ///< Update cells affected by the listed regions

            void updateSize();

            std::pair<CellIndex, CellIndex> getSize() const;

        public:

            RegionMap (Data& data);

            virtual int rowCount (const QModelIndex& parent = QModelIndex()) const;

            virtual int columnCount (const QModelIndex& parent = QModelIndex()) const;

            virtual QVariant data (const QModelIndex& index, int role = Qt::DisplayRole) const;

            virtual Qt::ItemFlags flags (const QModelIndex& index) const;

        private slots:

            void regionsAboutToBeRemoved (const QModelIndex& parent, int start, int end);

            void regionsInserted (const QModelIndex& parent, int start, int end);

            void regionsChanged (const QModelIndex& topLeft, const QModelIndex& bottomRight);

            void cellsAboutToBeRemoved (const QModelIndex& parent, int start, int end);

            void cellsInserted (const QModelIndex& parent, int start, int end);

            void cellsChanged (const QModelIndex& topLeft, const QModelIndex& bottomRight);
    };
}

#endif
