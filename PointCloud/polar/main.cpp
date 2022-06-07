#include <vtkActor.h>
#include <vtkCleanPolyData.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPointSource.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkInteractorStyleTrackballCamera.h>

using namespace std;

#define DEFAULT_DATA_FILE    "data.txt"

int main(int argc, char **argv)
{
    vtkSmartPointer<vtkPoints> m_Points = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkCellArray> vertices = vtkSmartPointer<vtkCellArray>::New();  //_存放细胞顶点，用于渲染（显示点云所必须的）
    vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
    vtkSmartPointer<vtkPolyDataMapper> pointMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    vtkSmartPointer<vtkActor> pointActor = vtkSmartPointer<vtkActor>::New();
    vtkSmartPointer<vtkRenderer> ren1 = vtkSmartPointer< vtkRenderer>::New();
    vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
    vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    vtkSmartPointer<vtkInteractorStyleTrackballCamera> istyle = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
 
    //_读进点云数据信息
    const char *filename = NULL;
    if (argc > 1) {
        filename = argv[1];
    }
    else {
        filename = DEFAULT_DATA_FILE;
    }
 
    ifstream infile(filename, ios::in);
    double x, y, z;
    double r, theta, phi;
    char line[128];
    int i = 0;
    while (infile.getline(line, sizeof(line)))
    {
        stringstream ss(line);
        ss >> r;
        ss >> theta;
        ss >> phi;

        /* convert polar to cartesian coordinates */
        x = r * std::sin(theta) * std::sin(phi);
        y = r * std::cos(theta);
        z = r * std::sin(theta) * std::cos(phi);
 
        m_Points->InsertPoint(i, x, y, z);  //_加入点信息
        vertices->InsertNextCell(1);        //_加入细胞顶点信息----用于渲染点集
        vertices->InsertCellPoint(i);
        i++;
    }
    infile.close();
 
    //_创建待显示数据源
 
    polyData->SetPoints(m_Points);      //_设置点集
    polyData->SetVerts(vertices);       //_设置渲染顶点
    pointMapper->SetInputData(polyData);
 
    pointActor->SetMapper(pointMapper);
    pointActor->GetProperty()->SetColor(1, 0, 0);
    pointActor->GetProperty()->SetAmbient(0.5);
    pointActor->GetProperty()->SetPointSize(3);
    //pointActor->GetProperty()->SetRepresentationToWireframe();
    //pointActor->GetProperty()->SetRepresentationToSurface();
 
    ren1->AddActor(pointActor);
    ren1->SetBackground(0, 0, 0);
 
    renWin->AddRenderer(ren1);
    renWin->SetSize(800, 800);
 
    iren->SetInteractorStyle(istyle);
    iren->SetRenderWindow(renWin);  //交互
 
    renWin->Render();
    iren->Start();
}