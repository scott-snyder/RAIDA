#include <RAIDA/IHistogram3DROOT.h>
#include <RAIDA/Naming.h>
#include <RAIDA/IAxisROOT.h>
#include <RAIDA/RAIDAUtil.h>
#include <RAIDA/AIDAHistogramsInROOT.h>
#include <iostream>
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TDirectory.h>
#include <string>
#include <iomanip>

using namespace AIDA ;
using namespace std;

IHistogram3DROOT::~IHistogram3DROOT()
{
    // The histograms (at least _histogram) are associated to a file.
    // Set the directory to 0 to remove this binding so we can delete them.
    _histogram->SetDirectory(0);
    delete _histogram;

    _histogramAIDA->SetDirectory(0);
    delete _histogramAIDA;

    _histogramAIDABinMeanX->SetDirectory(0);
    delete _histogramAIDABinMeanX;

    _histogramAIDABinMeanY->SetDirectory(0);
    delete _histogramAIDABinMeanY;

    _histogramAIDABinMeanZ->SetDirectory(0);
    delete _histogramAIDABinMeanZ;

    delete _xAxis;
    delete _yAxis;
    delete _zAxis;
}

IHistogram3DROOT::IHistogram3DROOT()
  : _histogram(0),  _histogramAIDA(0), _histogramAIDABinMeanX(0),
    _histogramAIDABinMeanY(0), _histogramAIDABinMeanZ(0),
    _xAxis(0),  _yAxis(0), _zAxis(0)
{
    // nothing to do, just created implementation to initialize pointers correctly
}

IHistogram3DROOT::IHistogram3DROOT(const std::string & name,
                                   const std::string & title,
                                   int nBinsX,
                                   double lowerEdgeX,
                                   double upperEdgeX,
                                   int nBinsY,
                                   double lowerEdgeY,
                                   double upperEdgeY,
                                   int nBinsZ,
                                   double lowerEdgeZ,
                                   double upperEdgeZ,
                                   const std::string & options)
{
  _histogram = new TH3D(name.c_str(),
			title.c_str(),
			(Int_t)nBinsX,
			(Axis_t)lowerEdgeX,(Axis_t)upperEdgeX,
                        (Int_t)nBinsY,
			(Axis_t)lowerEdgeY,(Axis_t)upperEdgeY,
                        (Int_t)nBinsZ,
			(Axis_t)lowerEdgeZ,(Axis_t)upperEdgeZ);
  _histogramAIDA = new TH3D(Naming::binEntry(name).c_str(),
                            Naming::titleBinEntry(title).c_str(),
			    (Int_t)nBinsX,
			    (Axis_t)lowerEdgeX,(Axis_t)upperEdgeX,
			    (Int_t)nBinsY,
			    (Axis_t)lowerEdgeY,(Axis_t)upperEdgeY,
			    (Int_t)nBinsZ,
			    (Axis_t)lowerEdgeZ,(Axis_t)upperEdgeZ);
  if (!AIDAHistogramsInROOT)
    _histogramAIDA->SetDirectory(0);
  _histogramAIDABinMeanX = new TH3D(Naming::binMeanX(name).c_str(),
				    Naming::titleBinMeanX(title).c_str(),
				    (Int_t)nBinsX,
				    (Axis_t)lowerEdgeX,(Axis_t)upperEdgeX,
				    (Int_t)nBinsY,
				    (Axis_t)lowerEdgeY,(Axis_t)upperEdgeY,
				    (Int_t)nBinsZ,
				    (Axis_t)lowerEdgeZ,(Axis_t)upperEdgeZ);
  if (!AIDAHistogramsInROOT)
    _histogramAIDABinMeanX->SetDirectory(0);
  _histogramAIDABinMeanY = new TH3D(Naming::binMeanY(name).c_str(),
				    Naming::titleBinMeanY(title).c_str(),
				    (Int_t)nBinsX,
				    (Axis_t)lowerEdgeX,(Axis_t)upperEdgeX,
				    (Int_t)nBinsY,
				    (Axis_t)lowerEdgeY,(Axis_t)upperEdgeY,
				    (Int_t)nBinsZ,
				    (Axis_t)lowerEdgeZ,(Axis_t)upperEdgeZ);
  if (!AIDAHistogramsInROOT)
    _histogramAIDABinMeanY->SetDirectory(0);
  _histogramAIDABinMeanZ = new TH3D(Naming::binMeanZ(name).c_str(),
				    Naming::titleBinMeanZ(title).c_str(),
				    (Int_t)nBinsX,
				    (Axis_t)lowerEdgeX,(Axis_t)upperEdgeX,
				    (Int_t)nBinsY,
				    (Axis_t)lowerEdgeY,(Axis_t)upperEdgeY,
				    (Int_t)nBinsZ,
				    (Axis_t)lowerEdgeZ,(Axis_t)upperEdgeZ);
  if (!AIDAHistogramsInROOT)
    _histogramAIDABinMeanZ->SetDirectory(0);
  // create axis
  auto xaxis = new IAxisROOT( _histogram->GetXaxis() );
  _xAxis = xaxis;
  xaxis->setFixedBinning();

  auto yaxis = new IAxisROOT( _histogram->GetYaxis() );
  _yAxis = yaxis;
  yaxis->setFixedBinning();

  auto zaxis = new IAxisROOT( _histogram->GetZaxis() );
  _zAxis = zaxis;
  zaxis->setFixedBinning();
}

IHistogram3DROOT::IHistogram3DROOT(const std::string & name,
				   const std::string & title,
				   const std::vector<double>  & binEdgesX,
				   const std::vector<double>  & binEdgesY,
				   const std::vector<double>  & binEdgesZ,
				   const std::string & options) 
{
  const int nBinsX = binEdgesX.size()-1;
  const int nBinsY = binEdgesY.size()-1;
  const int nBinsZ = binEdgesZ.size()-1;
  Double_t xBins[nBinsX+1];
  Double_t yBins[nBinsY+1];
  Double_t zBins[nBinsZ+1];
  for (int i=0;i<=nBinsX;i++)
    xBins[i] = binEdgesX[i];
  for (int i=0;i<=nBinsY;i++)
    yBins[i] = binEdgesY[i];
  for (int i=0;i<=nBinsZ;i++)
    zBins[i] = binEdgesZ[i];

  _histogram = new TH3D(name.c_str(),
			title.c_str(),
			(Int_t)nBinsX,xBins,
                        (Int_t)nBinsY,yBins,
                        (Int_t)nBinsZ,zBins);
  _histogramAIDA = new TH3D(Naming::binEntry(name).c_str(),
                            Naming::titleBinEntry(title).c_str(),
			    (Int_t)nBinsX,xBins,
			    (Int_t)nBinsY,yBins,
			    (Int_t)nBinsZ,zBins);
  if (!AIDAHistogramsInROOT)
    _histogramAIDA->SetDirectory(0);
  _histogramAIDABinMeanX = new TH3D(Naming::binMeanX(name).c_str(),
				    Naming::titleBinMeanX(title).c_str(),
				    (Int_t)nBinsX,xBins,
				    (Int_t)nBinsY,yBins,
				    (Int_t)nBinsZ,zBins);
  if (!AIDAHistogramsInROOT)
    _histogramAIDABinMeanX->SetDirectory(0);
  _histogramAIDABinMeanY = new TH3D(Naming::binMeanY(name).c_str(),
				    Naming::titleBinMeanY(title).c_str(),
				    (Int_t)nBinsX,xBins,
				    (Int_t)nBinsY,yBins,
				    (Int_t)nBinsZ,zBins);
  if (!AIDAHistogramsInROOT)
    _histogramAIDABinMeanY->SetDirectory(0);
  _histogramAIDABinMeanZ = new TH3D(Naming::binMeanZ(name).c_str(),
				    Naming::titleBinMeanZ(title).c_str(),
				    (Int_t)nBinsX,xBins,
				    (Int_t)nBinsY,yBins,
				    (Int_t)nBinsZ,zBins);
  if (!AIDAHistogramsInROOT)
    _histogramAIDABinMeanZ->SetDirectory(0);
  // create axis
  auto xaxis = new IAxisROOT( _histogram->GetXaxis() );
  _xAxis = xaxis;
  xaxis->setVariableBinning();

  auto yaxis = new IAxisROOT( _histogram->GetYaxis() );
  _yAxis = yaxis;
  yaxis->setVariableBinning();

  auto zaxis = new IAxisROOT( _histogram->GetZaxis() );
  _zAxis = zaxis;
  zaxis->setVariableBinning();
}

IHistogram3DROOT::IHistogram3DROOT(const std::string & name,
		 const IHistogram3DROOT & hist) 
{
  _histogram = (TH3D*)hist._histogram->Clone( name.c_str() );
  _histogramAIDA = (TH3D*)hist._histogramAIDA->Clone( Naming::binEntry(name).c_str() );
  if (!AIDAHistogramsInROOT)
    _histogramAIDA->SetDirectory(0);
  _histogramAIDABinMeanX = (TH3D*)hist._histogramAIDABinMeanX->Clone( Naming::binMeanX(name).c_str() );
  if (!AIDAHistogramsInROOT)
    _histogramAIDABinMeanX->SetDirectory(0);
  _histogramAIDABinMeanY = (TH3D*)hist._histogramAIDABinMeanY->Clone( Naming::binMeanY(name).c_str() );
  if (!AIDAHistogramsInROOT)
    _histogramAIDABinMeanY->SetDirectory(0);
  _histogramAIDABinMeanZ = (TH3D*)hist._histogramAIDABinMeanZ->Clone( Naming::binMeanZ(name).c_str() );
  if (!AIDAHistogramsInROOT)
    _histogramAIDABinMeanZ->SetDirectory(0);

  // create axis
  auto xaxis = new IAxisROOT( _histogram->GetXaxis() );
  _xAxis = xaxis;
  if ( hist._xAxis->isFixedBinning() )
    xaxis->setFixedBinning();
  else
    xaxis->setVariableBinning() ;

  auto yaxis = new IAxisROOT( _histogram->GetYaxis() );
  _yAxis = yaxis;
  if ( hist._yAxis->isFixedBinning() )
    yaxis->setFixedBinning();
  else
    yaxis->setVariableBinning() ;

  auto zaxis = new IAxisROOT( _histogram->GetZaxis() );
  _zAxis = zaxis;
  if ( hist._zAxis->isFixedBinning() )
    zaxis->setFixedBinning();
  else
    zaxis->setVariableBinning() ;
}


bool IHistogram3DROOT::fill(double x, double y, double z, double weight) 
{
  // only fill if weight is positiv
  if (weight<0) {  return false; }

  int iBin = (int)_histogram->FindBin( (Axis_t)x,(Axis_t)y,(Axis_t)z );
  double iBinHeight = (double)_histogram->GetBinContent(iBin);
  double newBinMeanX, oldBinMeanX;
  double newBinMeanY, oldBinMeanY;
  double newBinMeanZ, oldBinMeanZ;
  oldBinMeanX = (double)_histogramAIDABinMeanX->GetBinContent(iBin);
  newBinMeanX = (oldBinMeanX * iBinHeight + x*weight)/(weight+iBinHeight);
  oldBinMeanY = (double)_histogramAIDABinMeanY->GetBinContent(iBin);
  newBinMeanY = (oldBinMeanY * iBinHeight + y*weight)/(weight+iBinHeight);
  oldBinMeanZ = (double)_histogramAIDABinMeanZ->GetBinContent(iBin);
  newBinMeanZ = (oldBinMeanZ * iBinHeight + z*weight)/(weight+iBinHeight);

  _histogramAIDABinMeanX->SetBinContent( (Int_t)iBin, (Stat_t)newBinMeanX );
  _histogramAIDABinMeanY->SetBinContent( (Int_t)iBin, (Stat_t)newBinMeanY );
  _histogramAIDABinMeanZ->SetBinContent( (Int_t)iBin, (Stat_t)newBinMeanZ );
  _histogram->Fill( (Axis_t)x,(Axis_t)y,(Axis_t)z,(Stat_t)weight );
  _histogramAIDA->Fill( (Axis_t)x,(Axis_t)y,(Axis_t)z,(Stat_t)1. );
  return true;
}

double IHistogram3DROOT::binMeanX(int indexX, int indexY, int indexZ) const 
{
  int indexXROOT = RAIDAUtil::binIndexAIDA2ROOT(indexX,xAxis().bins());
  int indexYROOT = RAIDAUtil::binIndexAIDA2ROOT(indexY,yAxis().bins());
  int indexZROOT = RAIDAUtil::binIndexAIDA2ROOT(indexZ,zAxis().bins());

  return (double)_histogramAIDABinMeanX->GetBinContent( (Int_t)indexXROOT,
                                                        (Int_t)indexYROOT,
                                                        (Int_t)indexZROOT );
}

double IHistogram3DROOT::binMeanY(int indexX, int indexY, int indexZ) const 
{
  int indexXROOT = RAIDAUtil::binIndexAIDA2ROOT(indexX,xAxis().bins());
  int indexYROOT = RAIDAUtil::binIndexAIDA2ROOT(indexY,yAxis().bins());
  int indexZROOT = RAIDAUtil::binIndexAIDA2ROOT(indexZ,zAxis().bins());

  return (double)_histogramAIDABinMeanY->GetBinContent( (Int_t)indexXROOT,
                                                        (Int_t)indexYROOT,
                                                        (Int_t)indexZROOT );
}

double IHistogram3DROOT::binMeanZ(int indexX, int indexY, int indexZ) const 
{
  int indexXROOT = RAIDAUtil::binIndexAIDA2ROOT(indexX,xAxis().bins());
  int indexYROOT = RAIDAUtil::binIndexAIDA2ROOT(indexY,yAxis().bins());
  int indexZROOT = RAIDAUtil::binIndexAIDA2ROOT(indexZ,zAxis().bins());

  return (double)_histogramAIDABinMeanZ->GetBinContent( (Int_t)indexXROOT,
                                                        (Int_t)indexYROOT,
                                                        (Int_t)indexZROOT );
}

int IHistogram3DROOT::binEntries(int indexX, int indexY, int indexZ) const 
{
  int indexXROOT = RAIDAUtil::binIndexAIDA2ROOT(indexX,xAxis().bins());
  int indexYROOT = RAIDAUtil::binIndexAIDA2ROOT(indexY,yAxis().bins());
  int indexZROOT = RAIDAUtil::binIndexAIDA2ROOT(indexZ,zAxis().bins());

  return (int)_histogramAIDA->GetBinContent( (Int_t)indexXROOT,
					     (Int_t)indexYROOT,
					     (Int_t)indexZROOT );
}

int IHistogram3DROOT::binEntriesX(int index) const 
{
  int indexXROOT = RAIDAUtil::binIndexAIDA2ROOT(index,xAxis().bins());

  double sumX = 0;
  for ( int i=0 ; i<=((int)_histogramAIDA->GetNbinsY()+1) ; i++)
    {
      for ( int j=0 ; j<=((int)_histogramAIDA->GetNbinsZ()+1) ; j++)
	{
	  sumX += (double)_histogramAIDA->GetBinContent( (Int_t)indexXROOT,(Int_t)i,(Int_t)j );
	}
    }
  return (int)sumX;
}

int IHistogram3DROOT::binEntriesY(int index) const  
{
  int indexYROOT = RAIDAUtil::binIndexAIDA2ROOT(index,yAxis().bins());

  double sumY = 0;
  for ( int i=0 ; i<=((int)_histogramAIDA->GetNbinsX()+1) ; i++)
    {
      for ( int j=0 ; j<=((int)_histogramAIDA->GetNbinsZ()+1) ; j++)
	{
	  sumY += (double)_histogramAIDA->GetBinContent( (Int_t)i,(Int_t)indexYROOT,(Int_t)j );
	}
    }
  return (int)sumY;
}

int IHistogram3DROOT::binEntriesZ(int index) const 
{
  int indexZROOT = RAIDAUtil::binIndexAIDA2ROOT(index,zAxis().bins());

  double sumZ = 0;
  for ( int i=0 ; i<=((int)_histogramAIDA->GetNbinsX()+1) ; i++)
    {
      for ( int j=0 ; j<=((int)_histogramAIDA->GetNbinsY()+1) ; j++)
	{
	  sumZ += (double)_histogramAIDA->GetBinContent( (Int_t)i,(Int_t)j,(Int_t)indexZROOT );
	}
    }
  return (int)sumZ;
}

double IHistogram3DROOT::binHeight(int indexX, int indexY, int indexZ) const 
{
  int indexXROOT = RAIDAUtil::binIndexAIDA2ROOT(indexX,xAxis().bins());
  int indexYROOT = RAIDAUtil::binIndexAIDA2ROOT(indexY,yAxis().bins());
  int indexZROOT = RAIDAUtil::binIndexAIDA2ROOT(indexZ,zAxis().bins());

  return (double)_histogram->GetBinContent( (Int_t)indexXROOT,
					    (Int_t)indexYROOT,
					    (Int_t)indexZROOT );
}

double IHistogram3DROOT::binHeightX(int index) const 
{
  int indexXROOT = RAIDAUtil::binIndexAIDA2ROOT(index,xAxis().bins());

  double sumX = 0;
  for ( int i=0 ; i<=((int)_histogram->GetNbinsY()+1) ; i++)
    {
      for ( int j=0 ; j<=((int)_histogram->GetNbinsZ()+1) ; j++)
	{
	  sumX += (double)_histogram->GetBinContent( (Int_t)indexXROOT,(Int_t)i,(Int_t)j );
	}
    }
  return sumX;
}

double IHistogram3DROOT::binHeightY(int index) const 
{
  int indexYROOT = RAIDAUtil::binIndexAIDA2ROOT(index,yAxis().bins());

  double sumY = 0;
  for ( int i=0 ; i<=((int)_histogram->GetNbinsX()+1) ; i++)
    {
      for ( int j=0 ; j<=((int)_histogram->GetNbinsZ()+1) ; j++)
	{
	  sumY += (double)_histogram->GetBinContent( (Int_t)i,(Int_t)indexYROOT,(Int_t)j );
	}
    }
  return sumY;
}

double IHistogram3DROOT::binHeightZ(int index) const 
{
  int indexZROOT = RAIDAUtil::binIndexAIDA2ROOT(index,zAxis().bins());

  double sumZ = 0;
  for ( int i=0 ; i<=((int)_histogram->GetNbinsX()+1) ; i++)
    {
      for ( int j=0 ; j<=((int)_histogram->GetNbinsY()+1) ; j++)
	{
	  sumZ += (double)_histogram->GetBinContent( (Int_t)i,(Int_t)j,(Int_t)indexZROOT );
	}
    }
  return sumZ;
}

double IHistogram3DROOT::binError(int indexX, int indexY, int indexZ) const 
{
  int indexXROOT = RAIDAUtil::binIndexAIDA2ROOT(indexX,xAxis().bins());
  int indexYROOT = RAIDAUtil::binIndexAIDA2ROOT(indexY,yAxis().bins());
  int indexZROOT = RAIDAUtil::binIndexAIDA2ROOT(indexZ,zAxis().bins());

  return (double)_histogram->GetBinError( (Int_t)indexXROOT,
					  (Int_t)indexYROOT,
					  (Int_t)indexZROOT );
}

double IHistogram3DROOT::meanX() const 
{
  return (double)_histogram->GetMean( (Int_t)1 );
}

double IHistogram3DROOT::meanY() const 
{
  return (double)_histogram->GetMean( (Int_t)2 );
}

double IHistogram3DROOT::meanZ() const 
{
  return (double)_histogram->GetMean( (Int_t)3 );
}

double IHistogram3DROOT::rmsX() const 
{
  return (double)_histogram->GetRMS( (Int_t)1 );
}

double IHistogram3DROOT::rmsY() const 
{
  return (double)_histogram->GetRMS( (Int_t)2 );
}

double IHistogram3DROOT::rmsZ() const 
{
  return (double)_histogram->GetRMS( (Int_t)3 );
}

const IAxis & IHistogram3DROOT::xAxis() const
{
  return *_xAxis;
}

const IAxis & IHistogram3DROOT::yAxis() const
{
  return *_yAxis;
}

const IAxis & IHistogram3DROOT::zAxis() const
{
  return *_zAxis;
}

int IHistogram3DROOT::coordToIndexX(double coord) const 
{
  TH1 *xProj = _histogram->Project3D("x");
  int indexROOT = (int)xProj->FindBin( (Axis_t)coord );

  delete xProj;
  return RAIDAUtil::binIndexROOT2AIDA(indexROOT,xAxis().bins());
}

int IHistogram3DROOT::coordToIndexY(double coord) const 
{
  TH1 *yProj = _histogram->Project3D("y");
  int indexROOT = (int)yProj->FindBin( (Axis_t)coord );

  delete yProj;
  return RAIDAUtil::binIndexROOT2AIDA(indexROOT,yAxis().bins());
}

int IHistogram3DROOT::coordToIndexZ(double coord) const 
{
  TH1 *zProj = _histogram->Project3D("z");
  int indexROOT = (int)zProj->FindBin( (Axis_t)coord );

  delete zProj;
  return RAIDAUtil::binIndexROOT2AIDA(indexROOT,zAxis().bins());
}

// ---------------------------------------------------------------------------
// Functions from IBaseHistogram.h
// ---------------------------------------------------------------------------

std::string IHistogram3DROOT::title() const 
{
  string a(_histogram->GetTitle());
  return a;
}

bool IHistogram3DROOT::setTitle(const std::string & title) 
{
  _histogram->SetTitle( title.c_str() );
  _histogramAIDA->SetTitle( Naming::titleBinEntry(title).c_str() );
  _histogramAIDABinMeanX->SetTitle( Naming::titleBinMeanX(title).c_str() );
  _histogramAIDABinMeanY->SetTitle( Naming::titleBinMeanY(title).c_str() );
  _histogramAIDABinMeanZ->SetTitle( Naming::titleBinMeanZ(title).c_str() );
  return true;
}

bool IHistogram3DROOT::setName(const std::string & name) 
{
  _histogram->SetName( name.c_str() );
  _histogramAIDA->SetName( Naming::binEntry(name).c_str() );
  _histogramAIDABinMeanX->SetName( Naming::binMeanX(name).c_str() );
  _histogramAIDABinMeanY->SetName( Naming::binMeanY(name).c_str() );
  _histogramAIDABinMeanZ->SetName( Naming::binMeanZ(name).c_str() );
  return true;
}

int IHistogram3DROOT::dimension() const 
{
  return (int)_histogram->GetDimension();
}

bool IHistogram3DROOT::reset() 
{
  _histogram->Reset("");
  _histogramAIDA->Reset("");
  _histogramAIDABinMeanX->Reset("");
  _histogramAIDABinMeanY->Reset("");
  _histogramAIDABinMeanZ->Reset("");
  return true;
}

int IHistogram3DROOT::entries() const 
{
  double integral = 0;
  for ( int i=1 ; i<=(int)_histogramAIDA->GetNbinsX() ; i++ )
    {
      for ( int j=1 ; j<=(int)_histogramAIDA->GetNbinsY() ; j++ )
        {
	  for ( int k=1 ; k<=(int)_histogramAIDA->GetNbinsZ() ; k++ )
	    {
	      integral += (double)_histogramAIDA->GetBinContent( (Int_t)i,(Int_t)j,(Int_t)k );
	    }
        }
    }
  return (int)integral;
}

// ---------------------------------------------------------------------------
// Functions from IHistogram.h
// ---------------------------------------------------------------------------

int IHistogram3DROOT::allEntries() const 
{
  return (int)_histogramAIDA->GetEntries();
}

int IHistogram3DROOT::extraEntries() const 
{
  // xy-ebene mit 2*2 raendern
  // xz-ebene mit 1*2 Raendern entlang z
  // yz-ebene ohne rand

  double noExtra = 0.;

  // sum over xy-planes 
  for ( int i=0 ; i<=((int)_histogramAIDA->GetNbinsX()+1) ; i++ )
    {
      for ( int j=0 ; j<=((int)_histogramAIDA->GetNbinsY()+1) ; j++ )
	{
	  noExtra += (double)_histogramAIDA->GetBinContent( (Int_t)i,(Int_t)j,(Int_t)0 );
          noExtra += (double)_histogramAIDA->GetBinContent( (Int_t)i,(Int_t)j,((Int_t)_histogramAIDA->GetNbinsZ() + 1) );
	}
    }

  // sum over xz-planes 
  for ( int i=0 ; i<=((int)_histogramAIDA->GetNbinsX()+1) ; i++ )
    {
      for ( int j=1 ; j<=(int)_histogramAIDA->GetNbinsZ() ; j++ )
	{
	  noExtra += (double)_histogramAIDA->GetBinContent( (Int_t)i,(Int_t)0,(Int_t)j );
          noExtra += (double)_histogramAIDA->GetBinContent( (Int_t)i,((Int_t)_histogramAIDA->GetNbinsY() + 1),(Int_t)j );
	}
    }

  // sum over yz-planes 
  for ( int i=1 ; i<=(int)_histogramAIDA->GetNbinsY() ; i++ )
    {
      for ( int j=1 ; j<=(int)_histogramAIDA->GetNbinsZ() ; j++ )
	{
	  noExtra += (double)_histogramAIDA->GetBinContent( (Int_t)0,(Int_t)i,(Int_t)j );
          noExtra += (double)_histogramAIDA->GetBinContent( ((Int_t)_histogramAIDA->GetNbinsX() + 1),(Int_t)i,(Int_t)j );
	}
    }
  return (int)noExtra;
}

double IHistogram3DROOT::sumBinHeights() const 
{
  double integral = 0;
  for ( int i=1 ; i<=(int)_histogram->GetNbinsX() ; i++ )
    {
      for ( int j=1 ; j<=(int)_histogram->GetNbinsY() ; j++ )
        {
	  for ( int k=1 ; k<=(int)_histogram->GetNbinsZ() ; k++ )
	    {
	      integral += (double)_histogram->GetBinContent( (Int_t)i,(Int_t)j,(Int_t)k );
	    }
        }
    }
  return integral;
}

double IHistogram3DROOT::sumAllBinHeights() const 
{
  double integral = 0;
  for ( int i=0 ; i<=((int)_histogram->GetNbinsX()+1) ; i++ )
    {
      for ( int j=0 ; j<=((int)_histogram->GetNbinsY()+1) ; j++ )
        {
	  for ( int k=0 ; k<=((int)_histogram->GetNbinsZ()+1) ; k++ )
	    {
	      integral += (double)_histogram->GetBinContent( (Int_t)i,(Int_t)j,(Int_t)k );
	    }
        }
    }
  return integral;
}

double IHistogram3DROOT::sumExtraBinHeights() const 
{
  // xy-ebene mit 2*2 raendern
  // xz-ebene mit 1*2 Raendern entlang z
  // yz-ebene ohne rand

  double noExtra = 0.;

  // sum over xy-planes 
  for ( int i=0 ; i<=((int)_histogram->GetNbinsX()+1) ; i++ )
    {
      for ( int j=0 ; j<=((int)_histogram->GetNbinsY()+1) ; j++ )
	{
	  noExtra += (double)_histogram->GetBinContent( (Int_t)i,(Int_t)j,(Int_t)0 );
          noExtra += (double)_histogram->GetBinContent( (Int_t)i,(Int_t)j,((Int_t)_histogram->GetNbinsZ() + 1) );
	}
    }

  // sum over xz-planes 
  for ( int i=0 ; i<=((int)_histogram->GetNbinsX()+1) ; i++ )
    {
      for ( int j=1 ; j<=(int)_histogram->GetNbinsZ() ; j++ )
	{
	  noExtra += (double)_histogram->GetBinContent( (Int_t)i,(Int_t)0,(Int_t)j );
          noExtra += (double)_histogram->GetBinContent( (Int_t)i,((Int_t)_histogram->GetNbinsY() + 1),(Int_t)j );
	}
    }

  // sum over yz-planes 
  for ( int i=1 ; i<=(int)_histogram->GetNbinsY() ; i++ )
    {
      for ( int j=1 ; j<=(int)_histogram->GetNbinsZ() ; j++ )
	{
	  noExtra += (double)_histogram->GetBinContent( (Int_t)0,(Int_t)i,(Int_t)j );
          noExtra += (double)_histogram->GetBinContent( ((Int_t)_histogram->GetNbinsX() + 1),(Int_t)i,(Int_t)j );
	}
    }
  return noExtra;
}

double IHistogram3DROOT::minBinHeight() const 
{
  double minHeight;
  minHeight = (double)_histogram->GetBinContent( (Int_t)1,(Int_t)1,(Int_t)1 );

  for ( int i=1 ; i<=(int)_histogram->GetNbinsX() ; i++ )
    {
      for ( int j=1 ; j<=(int)_histogram->GetNbinsY() ; j++ )
        {
	  for ( int k=1 ; k<=(int)_histogram->GetNbinsZ() ; k++ )
	    {
	      if ( (double)_histogram->GetBinContent( (Int_t)i,(Int_t)j,(Int_t)k ) < minHeight)
		{
		  minHeight = (double)_histogram->GetBinContent( (Int_t)i,(Int_t)j,(Int_t)k );
		}
	    }
        }
    }
  return minHeight;
}

double IHistogram3DROOT::maxBinHeight() const 
{
  double maxHeight;
  maxHeight = (double)_histogram->GetBinContent( (Int_t)1,(Int_t)1,(Int_t)1 );

  for ( int i=1 ; i<=(int)_histogram->GetNbinsX() ; i++ )
    {
      for ( int j=1 ; j<=(int)_histogram->GetNbinsY() ; j++ )
        {
	  for ( int k=1 ; k<=(int)_histogram->GetNbinsZ() ; k++ )
	    {
	      if ( (double)_histogram->GetBinContent( (Int_t)i,(Int_t)j,(Int_t)k ) > maxHeight)
		{
		  maxHeight = (double)_histogram->GetBinContent( (Int_t)i,(Int_t)j,(Int_t)k );
		}
	    }
        }
    }
  return maxHeight;
}

bool IHistogram3DROOT::scale(double scaleFactor) 
{
  /// FIXME! richtige beruecksichtigung der Fehler im Zusammenhang mit dcale!
  _histogram->Scale( (Double_t)scaleFactor );
  return true;
}

bool IHistogram3DROOT::add(const IHistogram3D & hist) 
{
  if (xAxis().bins() != hist.xAxis().bins() ) return false;
  if (yAxis().bins() != hist.yAxis().bins() ) return false;
  if (zAxis().bins() != hist.zAxis().bins() ) return false;

  IHistogram3DROOT const * localhist = dynamic_cast<const IHistogram3DROOT*>(&hist);

  for (int i=0 ; i<=_histogramAIDABinMeanX->GetNbinsX()+1 ; i++)
    {
      for (int j=0 ; j<=_histogramAIDABinMeanX->GetNbinsY()+1 ; j++)
	{
	  for (int k=0 ; k<=_histogramAIDABinMeanX->GetNbinsZ()+1 ; k++)
	    {
	      double binMeanX1 = (double)_histogramAIDABinMeanX->GetBinContent(i,j,k);
	      double binMeanX2 = (double)localhist->_histogramAIDABinMeanX->GetBinContent(i,j,k);
	      double binMeanY1 = (double)_histogramAIDABinMeanY->GetBinContent(i,j,k);
	      double binMeanY2 = (double)localhist->_histogramAIDABinMeanY->GetBinContent(i,j,k);
	      double binMeanZ1 = (double)_histogramAIDABinMeanZ->GetBinContent(i,j,k);
	      double binMeanZ2 = (double)localhist->_histogramAIDABinMeanZ->GetBinContent(i,j,k);

	      double weight1 = (double)_histogram->GetBinContent(i,j,k);
	      double weight2 = (double)localhist->_histogram->GetBinContent(i,j,k);

	      double newBinMeanX = 
		(binMeanX1 * weight1 + binMeanX2 * weight2)/(weight1+weight2);
	      double newBinMeanY = 
		(binMeanY1 * weight1 + binMeanY2 * weight2)/(weight1+weight2);
	      double newBinMeanZ = 
		(binMeanZ1 * weight1 + binMeanZ2 * weight2)/(weight1+weight2);
	      _histogramAIDABinMeanX->SetBinContent( (Int_t)i,(Int_t)j,(Int_t)k, (Double_t)newBinMeanX );
	      _histogramAIDABinMeanY->SetBinContent( (Int_t)i,(Int_t)j,(Int_t)k, (Double_t)newBinMeanY );
	      _histogramAIDABinMeanZ->SetBinContent( (Int_t)i,(Int_t)j,(Int_t)k, (Double_t)newBinMeanZ );
	    }
	}
    }
  _histogram->Add(localhist->_histogram);
  _histogramAIDA->Add(localhist->_histogramAIDA);

  return true;
}

bool IHistogram3DROOT::subtract(const IHistogram3D & hist) 
{
  if ( xAxis().bins() != hist.xAxis().bins() ) return false;
  if ( yAxis().bins() != hist.yAxis().bins() ) return false;
  if ( zAxis().bins() != hist.zAxis().bins() ) return false;
  IHistogram3DROOT const * localhist = dynamic_cast<const IHistogram3DROOT*>(&hist);

  for (int i=0 ; i<=_histogramAIDABinMeanX->GetNbinsX()+1 ; i++)
    {
      for (int j=0 ; j<=_histogramAIDABinMeanX->GetNbinsY()+1 ; j++)
	{
	  for (int k=0 ; k<=_histogramAIDABinMeanX->GetNbinsZ()+1 ; k++)
	    {
	      double binMeanX1 = (double)_histogramAIDABinMeanX->GetBinContent(i,j,k);
	      double binMeanX2 = (double)localhist->_histogramAIDABinMeanX->GetBinContent(i,j,k);
	      double binMeanY1 = (double)_histogramAIDABinMeanY->GetBinContent(i,j,k);
	      double binMeanY2 = (double)localhist->_histogramAIDABinMeanY->GetBinContent(i,j,k);
	      double binMeanZ1 = (double)_histogramAIDABinMeanZ->GetBinContent(i,j,k);
	      double binMeanZ2 = (double)localhist->_histogramAIDABinMeanZ->GetBinContent(i,j,k);

	      double weight1 = (double)_histogram->GetBinContent(i,j,k);
	      double weight2 = (double)localhist->_histogram->GetBinContent(i,j,k);

	      double newBinMeanX, newBinMeanY, newBinMeanZ;
	      if ( (weight1-weight2) )
		{
		  newBinMeanX = 
		    (binMeanX1 * weight1 - binMeanX2 * weight2)/(weight1-weight2);
		  newBinMeanY = 
		    (binMeanY1 * weight1 - binMeanY2 * weight2)/(weight1-weight2);
		  newBinMeanZ = 
		    (binMeanZ1 * weight1 - binMeanZ2 * weight2)/(weight1-weight2);
		}
	      else
		{
		  newBinMeanX = 0; 
		  newBinMeanY = 0; 
		  newBinMeanZ = 0; 
		}
	      _histogramAIDABinMeanX->SetBinContent( (Int_t)i,(Int_t)j,(Int_t)k, (Double_t)newBinMeanX );
	      _histogramAIDABinMeanY->SetBinContent( (Int_t)i,(Int_t)j,(Int_t)k, (Double_t)newBinMeanY );
	      _histogramAIDABinMeanZ->SetBinContent( (Int_t)i,(Int_t)j,(Int_t)k, (Double_t)newBinMeanZ );
	    }
	}
    }

  _histogram->Add(localhist->_histogram,-1.);
  _histogramAIDA->Add(localhist->_histogramAIDA,-1.);

  return true;
}

bool IHistogram3DROOT::multiply(const IHistogram3D & hist) 
{
  if ( xAxis().bins() != hist.xAxis().bins() ) return false;
  if ( yAxis().bins() != hist.yAxis().bins() ) return false;
  if ( zAxis().bins() != hist.zAxis().bins() ) return false;
  IHistogram3DROOT const * localhist = dynamic_cast<const IHistogram3DROOT*>(&hist);

  for (int i=0 ; i<=_histogramAIDABinMeanX->GetNbinsX()+1 ; i++)
    {
      for (int j=0 ; j<=_histogramAIDABinMeanX->GetNbinsY()+1 ; j++)
	{
	  for (int k=0 ; k<=_histogramAIDABinMeanX->GetNbinsZ()+1 ; k++)
	    {
	      _histogramAIDABinMeanX->SetBinContent( (Int_t)i,(Int_t)j,(Int_t)k, (Double_t)0 );
	      _histogramAIDABinMeanY->SetBinContent( (Int_t)i,(Int_t)j,(Int_t)k, (Double_t)0 );
	      _histogramAIDABinMeanZ->SetBinContent( (Int_t)i,(Int_t)j,(Int_t)k, (Double_t)0 );
	    }
	}
    }

  _histogram->Multiply(localhist->_histogram);
  _histogramAIDA->Multiply(localhist->_histogramAIDA);

  return true;
}

bool IHistogram3DROOT::divide(const IHistogram3D & hist) 
{
  if ( xAxis().bins() != hist.xAxis().bins() ) return false;
  if ( yAxis().bins() != hist.yAxis().bins() ) return false;
  if ( zAxis().bins() != hist.zAxis().bins() ) return false;
  IHistogram3DROOT const * localhist = dynamic_cast<const IHistogram3DROOT*>(&hist);

  for (int i=0 ; i<=_histogramAIDABinMeanX->GetNbinsX()+1 ; i++)
    {
      for (int j=0 ; j<=_histogramAIDABinMeanX->GetNbinsY()+1 ; j++)
	{
	  for (int k=0 ; k<=_histogramAIDABinMeanX->GetNbinsZ()+1 ; k++)
	    {
	      _histogramAIDABinMeanX->SetBinContent( (Int_t)i,(Int_t)j,(Int_t)k, (Double_t)0 );
	      _histogramAIDABinMeanY->SetBinContent( (Int_t)i,(Int_t)j,(Int_t)k, (Double_t)0 );
	      _histogramAIDABinMeanZ->SetBinContent( (Int_t)i,(Int_t)j,(Int_t)k, (Double_t)0 );
	    }
	}
    }

  _histogram->Divide(localhist->_histogram);
  _histogramAIDA->Divide(localhist->_histogramAIDA);

  return true;
}

void IHistogram3DROOT::printContents() const 
{
  int colWidth = 10;

  cout << "*******************************************************************************" << endl;
  cout << endl 
       << "Contents of 3D histogram:" << endl  
       << "------------------------" << endl << endl; 
  cout << "title:              " << title() << endl; 

  cout << "x-range:            " 
       << "min: " << xAxis().lowerEdge()
       << ", max: " << xAxis().upperEdge() 
       << ", nBins: " << xAxis().bins() ;
  if (xAxis().isFixedBinning()) cout << " (fixed " ;
  else cout << " (variable " ;
  cout << "binning)" << endl; 
  cout << "y-range:            " 
       << "min: " << yAxis().lowerEdge()
       << ", max: " << yAxis().upperEdge() 
       << ", nBins: " << yAxis().bins() ;
  if (yAxis().isFixedBinning()) cout << " (fixed " ;
  else cout << " (variable " ;
  cout << "binning)" << endl; 
  cout << "z-range:            " 
       << "min: " << zAxis().lowerEdge()
       << ", max: " << zAxis().upperEdge() 
       << ", nBins: " << zAxis().bins() ;
  if (zAxis().isFixedBinning()) cout << " (fixed " ;
  else cout << " (variable " ;
  cout << "binning)" << endl; 

  cout << "entries:            " 
       << "in-range:  " << entries() << endl; 
  cout << "                    " 
       << "x-UNDERFLOW: " << binEntriesX(-2) 
       << " x-OVERFLOW: " << binEntriesX(-1) << endl;
  cout << "                    " 
       << "y-UNDERFLOW: " << binEntriesY(-2) 
       << " y-OVERFLOW: " << binEntriesY(-1) << endl; 
  cout << "                    " 
       << "z-UNDERFLOW: " << binEntriesZ(-2) 
       << " z-OVERFLOW: " << binEntriesZ(-1)
       << " (total: " << extraEntries() << ")" << endl; 
  cout << "                    " 
       << "total:     " << allEntries() << endl; 
  cout << "sum of bin heights: " 
       << "in-range:  " << sumBinHeights() << endl; 
  cout << "                    " 
       << "x-UNDERFLOW: " << binHeightX(-2)
       << " x-OVERFLOW: " << binHeightX(-1) << endl;
  cout << "                    " 
       << "y-UNDERFLOW: " << binHeightY(-2)
       << " y-OVERFLOW: " << binHeightY(-1) << endl; 
  cout << "                    " 
       << "z-UNDERFLOW: " << binHeightZ(-2)
       << " z-OVERFLOW: " << binHeightZ(-1)
       << " (total: " << sumExtraBinHeights() << ")" << endl; 
  cout << "                    " 
       << "sum:       " << sumAllBinHeights()<< endl; 
  cout << "bin height:         " << "minimum:   " << minBinHeight()
       << " maximum: " << maxBinHeight() << endl;
  cout << "statistics:         " 
       << "x-mean: " << meanX() 
       << " x-RMS: " << rmsX() 
       << " y-mean: " << meanY() 
       << " y-RMS: " << rmsY() 
       << " z-mean: " << meanZ() 
       << " z-RMS: " << rmsZ() << endl;
  cout << endl; 

  cout << "bin contents:" << endl 
       << "------------" << endl << endl;
  cout << "   x-binID | "
       << "   y-binID | "
       << "   z-binID | "
       << "   x-low edge | "
       << "   y-low edge | "
       << "   z-low edge | "
       << "   entries | "
       << "x-bin mean | " 
       << "y-bin mean | " 
       << "z-bin mean | " 
       << "     hight | " 
       << "     error" << endl; 
  cout << "------------------------------------------------------------------------------" << endl;

  for (int i=IAxis::UNDERFLOW_BIN; i<xAxis().bins(); i++)
    {
      for (int j=IAxis::UNDERFLOW_BIN; j<yAxis().bins(); j++)
	{
	  for (int k=IAxis::UNDERFLOW_BIN; k<zAxis().bins(); k++)
	    {
	      cout << setw(colWidth) << i << " | " ;
	      cout << setw(colWidth) << j << " | " ;
	      cout << setw(colWidth) << k << " | " ;
	      cout << setw(colWidth+3) << xAxis().binLowerEdge(i) << " | " ;
	      cout << setw(colWidth+3) << yAxis().binLowerEdge(j) << " | " ;
	      cout << setw(colWidth+3) << zAxis().binLowerEdge(k) << " | " ;
	      cout << setw(colWidth) << binEntries(i,j,k) << " | " 
		   << setw(colWidth) << binMeanX(i,j,k) << " | " 
		   << setw(colWidth) << binMeanY(i,j,k) << " | " 
		   << setw(colWidth) << binMeanZ(i,j,k) << " | " 
		   << setw(colWidth) << binHeight(i,j,k) << " | " 
		   << setw(colWidth) << binError(i,j,k) << endl;
	    }
	}
    }
  cout << "------------------------------------------------------------------------------" << endl;

  cout << "*******************************************************************************" << endl;
}
