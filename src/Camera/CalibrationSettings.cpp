#include "Camera\CalibrationSettings.h"

namespace Camera
{

void CalibrationSettings::Write(cv::FileStorage& p_fs)   
	{
		p_fs << "{" << "BoardSize_Width"  << m_boardSize.width
			<< "BoardSize_Height" << m_boardSize.height
			<< "Square_Size"         << m_squareSize
			<< "Calibrate_Pattern" << m_patternToUse
			<< "Calibrate_NrOfFrameToUse" << m_nrFrames
			<< "Calibrate_FixAspectRatio" << m_aspectRatio
			<< "Calibrate_AssumeZeroTangentialDistortion" << m_calibZeroTangentDist
			<< "Calibrate_FixPrincipalPointAtTheCenter" << m_calibFixPrincipalPoint

			<< "Write_DetectedFeaturePoints" << m_bwritePoints
			<< "Write_extrinsicParameters"   << m_bwriteExtrinsics
			<< "Write_outputFileName"  << m_outputFileName

			<< "Show_UndistortedImage" << m_showUndistorsed

			<< "Input_FlipAroundHorizontalAxis" << m_flipVertical
			<< "Input_Delay" << m_delay
			<< "Input" << m_input
			<< "}";
	}

	void CalibrationSettings::Read(const cv::FileNode& P_NODE)                          //Read serialization for this class
	{
		P_NODE["BoardSize_Width" ] >> m_boardSize.width;
		P_NODE["BoardSize_Height"] >> m_boardSize.height;
		P_NODE["Calibrate_Pattern"] >> m_patternToUse;
		P_NODE["Square_Size"]  >> m_squareSize;
		P_NODE["Calibrate_NrOfFrameToUse"] >> m_nrFrames;
		P_NODE["Calibrate_FixAspectRatio"] >> m_aspectRatio;
		P_NODE["Write_DetectedFeaturePoints"] >> m_bwritePoints;
		P_NODE["Write_extrinsicParameters"] >> m_bwriteExtrinsics;
		P_NODE["Write_outputFileName"] >> m_outputFileName;
		P_NODE["Calibrate_AssumeZeroTangentialDistortion"] >> m_calibZeroTangentDist;
		P_NODE["Calibrate_FixPrincipalPointAtTheCenter"] >> m_calibFixPrincipalPoint;
		P_NODE["Input_FlipAroundHorizontalAxis"] >> m_flipVertical;
		P_NODE["Show_UndistortedImage"] >> m_showUndistorsed;
		P_NODE["Input"] >> m_input;
		P_NODE["Input_Delay"] >> m_delay;
		Interprate();
	}

	void CalibrationSettings::Interprate()
	{
		m_goodInput = true;
		if (m_boardSize.width <= 0 || m_boardSize.height <= 0)
		{
			std::cerr << "Invalid Board size: " << m_boardSize.width << " " << m_boardSize.height << std::endl;
			m_goodInput = false;
		}
		if (m_squareSize <= 10e-6)
		{
			std::cerr << "Invalid square size " << m_squareSize << std::endl;
			m_goodInput = false;
		}
		if (m_nrFrames <= 0)
		{
			std::cerr << "Invalid number of frames " << m_nrFrames << std::endl;
			m_goodInput = false;
		}

		if (m_input.empty())      // Check for valid input
			m_inputType = INVALID;
		else
		{
			if (m_input[0] >= '0' && m_input[0] <= '9')
			{
				std::stringstream ss(m_input);
				ss >> m_cameraID;
				m_inputType = CAMERA;
			}
			else
			{
				if (ReadStringList(m_input, m_imageList))
				{
					m_inputType = IMAGE_LIST;
					m_nrFrames = (m_nrFrames < (int)m_imageList.size()) ? m_nrFrames : (int)m_imageList.size();
				}
				else
					m_inputType = VIDEO_FILE;
			}
			if (m_inputType == CAMERA)
				m_inputCapture.open(m_cameraID);
			if (m_inputType == VIDEO_FILE)
				m_inputCapture.open(m_input);
			if (m_inputType != IMAGE_LIST && !m_inputCapture.isOpened())
				m_inputType = INVALID;
		}
		if (m_inputType == INVALID)
		{
			std::cerr << " Inexistent input: " << m_input;
			m_goodInput = false;
		}

		m_flag = 0;
		if(m_calibFixPrincipalPoint) m_flag |= CV_CALIB_FIX_PRINCIPAL_POINT;
		if(m_calibZeroTangentDist)   m_flag |= CV_CALIB_ZERO_TANGENT_DIST;
		if(m_aspectRatio)            m_flag |= CV_CALIB_FIX_ASPECT_RATIO;


		m_calibrationPattern = NOT_EXISTING;
		if (!m_patternToUse.compare("CHESSBOARD")) m_calibrationPattern = CHESSBOARD;
		if (!m_patternToUse.compare("CIRCLES_GRID")) m_calibrationPattern = CIRCLES_GRID;
		if (!m_patternToUse.compare("ASYMMETRIC_CIRCLES_GRID")) m_calibrationPattern = ASYMMETRIC_CIRCLES_GRID;
		if (m_calibrationPattern == NOT_EXISTING)
		{
			std::cerr << " Inexistent camera calibration mode: " << m_patternToUse << std::endl;
			m_goodInput = false;
		}
		m_atImageList = 0;
	}

	cv::Mat CalibrationSettings::NextImage()
	{
		cv::Mat result;
		if( m_inputCapture.isOpened() )
		{
			cv::Mat view0;
			m_inputCapture >> view0;
			view0.copyTo(result);
		}
		else if( m_atImageList < (int)m_imageList.size() )
			result = cv::imread(m_imageList[m_atImageList++], CV_LOAD_IMAGE_COLOR);

		return result;
	}

	bool CalibrationSettings::ReadStringList( const std::string& P_FILENAME, std::vector<std::string>& p_l )
	{
		p_l.clear();
		cv::FileStorage fs(P_FILENAME, cv::FileStorage::READ);
		if( !fs.isOpened() )
			return false;
		cv::FileNode n = fs.getFirstTopLevelNode();
		if( n.type() != cv::FileNode::SEQ )
			return false;
		cv::FileNodeIterator it = n.begin(), it_end = n.end();
		for( ; it != it_end; ++it )
			p_l.push_back((std::string)*it);
		return true;
	}

}
