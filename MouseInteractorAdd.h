#ifndef MOUSERINTERACTORADD_H
#define MOUSERINTERACTORADD_H


#include <vtkGenericDataObjectReader.h>

#include <vtkPolyDataMapper.h>
#include <vtkQtTableView.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkSphereSource.h>
#include <vtkSmartPointer.h>
#include <vtkEventQtSlotConnect.h>
#include <vtkInteractorStyleTrackballActor.h>
#include <vtkProperty.h>

#include <vtkVersion.h>
#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkSphereSource.h>
#include <vtkRendererCollection.h>
#include <vtkCellArray.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkObjectFactory.h>
#include <vtkPlaneSource.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkPropPicker.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPointPicker.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkPolyLine.h>
#include <vtkCellArray.h>
#include <vtkDoubleArray.h>
#include <vtkCellData.h>
#include <vtkCleanPolyData.h>
#include <vtkDataSetMapper.h>
#include <vtkExtractGeometry.h>
#include <vtkAreaPicker.h>

#include <vtkInteractorStyleSwitch.h>
#include <vtkDoubleArray.h>
#include <vtkAppendPolyData.h>

#include <vtkDelaunay3D.h>
#include <vtkDelaunay2D.h>
#include <vtkShrinkFilter.h>
#include <vtkTriangle.h>
#include <vtkPointData.h>
#include <vtkPolygon.h>
#include <vtkPolyDataReader.h>
#include <vtkGenericDataObjectWriter.h>
#include <sstream>
#include <vtkPolyDataNormals.h>
#include <vtkFloatArray.h>
#include <vtkMath.h>
#include <vtkLineSource.h>
#include <vtkArrowSource.h>

#include <vtkAffineWidget.h>
#include <vtkAffineRepresentation2D.h>
#include <vtkCommand.h>
#include <vtkTransform.h>
#include <vtkObject.h>
#include <vtkDecimatePro.h>
#include <vtkTriangleFilter.h>
#include <vtkQuadricDecimation.h>

#include <QtGui>

#include "constants.h"
//#include "EventQtSlotConnect.h"

// Handle mouse events
class MouseInteractorAdd : public vtkInteractorStyleTrackballCamera
{
public:
	static MouseInteractorAdd* New();
	vtkTypeMacro(MouseInteractorAdd, vtkInteractorStyleTrackballCamera);

	MouseInteractorAdd();

	double Distance(double p1[3], double p2[3]);
	void SetSelectedTriColor();
	int ConstrainEdge(int type1, int type2);
	int PairNumber(int a, int b);
	int DrawTriangle();
	void DeleteTriangle(double*);
	vtkActor* PickActorFromMesh(double pos[3]);
	vtkActor* PickActorFromTriangle(double pos[3]);
	void FlipNormal(double*);
	void AddPoint(double*);
	void DeletePoint(double*);
	int PickPointForTri(double*);
	void CheckNormal(int triId[3]);
	void AddDecimateEdge(int pointSeq);

	void copyEdgeBtoA(int a, int b);
	int deleteEdgeHelper(int id1, int id2, int seq);
	int deleteEdgeHelper2(int id, int seq);
	void deleteEdge(int seq);
	void setNormalGenerator(vtkSmartPointer<vtkPolyDataNormals> normalGenerator);
	void setLabelTriNum();
	void setLabelPtNum();
	void reset();

	void DrawDelaunayTriangle();
	void AutoTriangulation();
	
	virtual void OnLeftButtonDown()
	{
		int* clickPos = new int[2];
		clickPos = this->GetInteractor()->GetEventPosition();
		// Pick from this location.
		vtkSmartPointer<vtkPropPicker>  picker =
			vtkSmartPointer<vtkPropPicker>::New();

		int con = 1;

		int sucessPick = picker->Pick(clickPos[0], clickPos[1], 0, this->GetDefaultRenderer());
		if(sucessPick != 0)//pick successful
		{
			double* pos = new double[3];
			pos = picker->GetPickPosition();			
			vtkRenderWindowInteractor *rwi = this->Interactor;

			if(rwi->GetKeySym() != NULL && isSkeleton)
			{
				std::string key = rwi->GetKeySym();
				//for the adding point event
				if( rwi->GetControlKey() && pos[0] != 0 && pos[1] != 0 && pos[2] != 0)
				{
					reset();
					AddPoint(pos);
				}				
				else if(key.compare("s") == 0 || key.compare("S") == 0)
				{
					reset();
					DeletePoint(pos);	//for delete point event				
				}								
				else if(key.compare("q") == 0 || key.compare("Q") == 0 || drawTriMode)
				{
					drawTriMode = true;
					con = PickPointForTri(pos);
					if(con == 0){
						QMessageBox messageBox;
						messageBox.critical(0,"Error","Triangle Violation");
					}
				}
				else if(key.compare("b") == 0 || key.compare("B") == 0)
				{
					reset();
					FlipNormal(pos);
				}
				else if(key.compare("d") == 0 || key.compare("D") == 0)
				{
					reset();
					DeleteTriangle(pos);					
				}
			}//end of key press
		}		
		if(con != 0)
			vtkInteractorStyleTrackballCamera::OnLeftButtonDown();		
	}

	void OnKeyRelease()
	{
		std::string key = this->Interactor->GetKeySym();
		if(key.compare("q") != 0 && key.compare("Q") != 0)
			this->Interactor->SetKeySym("");
	}


	std::vector<int>deletePointIds;
	std::vector<int>triPtIds;

	vtkActor *selectedTriangle;
	vtkActor *prePolyLineActor;

	QLabel *labelPtNumber;
	QLabel *labelTriNumber;

	static std::vector<TagInfo> vectorTagInfo;
	static std::vector<TagTriangle> vectorTagTriangles;
	static std::vector<TagPoint> vectorTagPoints;
	static std::vector<std::vector<TagPoint>> vectorClassifyPoints;
	static std::vector<TagEdge> vectorTagEdges;
	//store all the label info, 0 represent no tag on this point
	static std::vector<double> labelData;
	static std::vector<vtkActor*> triNormalActors;
	static bool isSkeleton;
	static int selectedTag;
	static double targetReduction;
	static double featureAngle;

	static double triCol[3];
	static double backCol[3];

	static bool decimateMode;


private:
	bool drawTriMode;
	vtkSmartPointer<vtkPolyDataNormals> normalGenerator;
};
#endif