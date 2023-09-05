#ifndef CSegyReadWrite_H
#define CSegyReadWrite_H

#include <memory>
#include <QBuffer>
#include <QTableWidget>

#include "csegydlg.h"
#include "cSeisData.h"
#include "cerrwarninfo.h"
#include "cSeisHdrTableView.h"

//namespace GnV //GnGViewer
//{
	constexpr quint64 EBCHDR = 3200;
	constexpr quint64 BINHDR = 400;
	constexpr quint64 TRCHDR = 240;

	class cSegYReadWrite
	{

	public:
		cSegYReadWrite();
		//QVector<cHdrBytLoc> m_RetHdrBytLoc;

		void FindTraceStride(std::shared_ptr<cSeisData> pSeisData, quint64 numSampEachTrc, quint32 dataFormat);
		bool FindNumOfTraces(std::shared_ptr<cSeisData> pSeisData, quint64 fileSize);
		void StoreSegYTemplate(std::shared_ptr<cSeisData> pSeisData, QTableWidget* BinTblWgt, QTableWidget* TrcTblWgt);
		void PopulateTraceHdrs(std::shared_ptr<cSeisData> pSeisData, const QString& file);
		bool ReadEBCDIChdr(std::shared_ptr<cSeisData> pSeisData, QFile* inSegyfile);
		bool ReadBinaryHeader(std::shared_ptr<cSeisData> pSeisData, QFile* inSegyfile);
		bool ReadTrcHdr(std::shared_ptr<cSeisData> pSeisData, QString& file, quint64& frmTrc, quint64& toTrc);
		bool GetTraceSamples(long& DataFrmt, QString& strSegyfile, std::shared_ptr <cSeisGather> seisGth, QVector<float>& vecSamp, quint64 frmTrc, quint64 toTrc);
		bool GetTraceSamples(long& DataFrmt, QString& strSegyfile, std::shared_ptr<cSeisGather> seisGth, QVector<float>& vecSamp);
		bool TRACEReader_IBM2IEEE(QString& strSegyfile, std::shared_ptr<cSeisGather> seisGth, quint64& frmTrc, quint64& toTrc, QVector<float>& vecSamp);
		bool TRACEReader_RawBytes2IEEE(QString& strSegyfile, std::shared_ptr<cSeisGather> seisGth, quint64& frmTrc, quint64& toTrc, QVector<float>& vecSamp);
		void ApplySpatialScalars(std::shared_ptr<cSeisData> pSeisData);
		void ApplyDepthScalars(std::shared_ptr<cSeisData> pSeisData);
		bool RunPreCheck(std::shared_ptr<cSeisData> pSeisData, quint64 frmTrc, quint64 toTrc);
		void ExportHdrsToCSV(std::shared_ptr<cSeisData> pSeisData, QString fName, char delimiter);
		bool IsDblTrcHdr(QString& name);


	private:
		bool m_IsPreCheckDone = false;
		QStringList m_lst
		{ 
			"RcvGrpElev",
			"SurfElevAtSrc",
			"SrcDpthBlowSurf",
			"DatmElevAtRcvGrp",
			"DatmElevAtSrc",
			"WaterDpthAtSrcGrp",
			"WaterDpthArRcvGrp",
			"XsrcCoord",
			"YsrcCoord",
			"XrcvGrpCoord",
			"YrcvGrpCoord" 
		};

	};
//}
#endif //CSegyReadWrite_H

