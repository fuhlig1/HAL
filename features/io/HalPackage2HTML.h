/*
 * HalPackage2HTML.h
 *
 *  Created on: 30-04-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef HALPACKAGE2HTML_H_
#define HALPACKAGE2HTML_H_

#include "HalStd.h"

#include <TObject.h>
#include <TString.h>
#include <initializer_list>
#include <vector>


class HalHtmlFile;
class HalHtmlObject;
class HalPackage;
class TGraph;
class TObjString;

class TH1;
class TH2;
class TH3;
/**
 * make HTML reports
 */
class HalPackage2HTML : public TObject {
  friend class HalAnaFile;

protected:
  enum class eTableStyle { kStandard, kMetaData };

private:
  TString AddToUrl(TString add_dir, TString url) const;
  void GetCollectionsNumbers();
  void CreateImagePage(TString filename, TString image_flag, Int_t no, TString path = "") const;
  TString GetLinkCutMon(TH1* h1, TH1* h2, Int_t no, TString path = "") const;
  TString GetLink1D(TH1* h1, TH1* h2, Int_t no, TString path = "") const;
  TString GetLink2D(TH2* h1, TH2* h2, Int_t no, TString path = "") const;
  TString GetLink3D(TH3* h1, TH3* h2, Int_t no, TString path = "") const;
  TString GetLinkToCut(Hal::ECutUpdate update, Int_t collection_no, Int_t cut_no, Bool_t fast) const;
  TString GetLinkToCutMonitor(Hal::ECutUpdate udpate, Int_t collection_no, Int_t cut_mon_no) const;
  void CreatePackageList(HalHtmlObject& table,
                         HalPackage* pack,
                         eTableStyle style,
                         TString path,
                         Int_t depth,
                         Option_t* opt = "drawcomment+drawclassname");
  void CreateCutAndMonitorList(HalHtmlObject& table, TString path);
  void ExportCollections(HalHtmlObject& object, TString path);
  void CreateCutHTML(HalHtmlObject& table, Hal::ECutUpdate cut_upd, Int_t collection_no);
  void CreateCutLink(HalHtmlObject& table, Hal::ECutUpdate cut_update, HalPackage* obj, Int_t no, Bool_t fast);
  void CreateCutMonitorLink(HalHtmlObject& table, Hal::ECutUpdate update, HalPackage* obj, Int_t no, Int_t counter, TString path);
  void CreateCutMonitorHTML(HalHtmlObject& table, Hal::ECutUpdate cut_upd, Int_t collection_no, Int_t& counter, TString path);
  void CreateListTable(HalHtmlObject& table, TList* list, Int_t no, TString path_data, TString path_url, TString drawClass);
  void DrawContainer(HalPackage* pack,
                     Int_t tier_no,
                     Int_t tier,
                     Int_t collections_from_no,
                     Int_t collections_to_no,
                     Int_t layer      = 0,
                     Int_t line_style = 0,
                     Int_t tier_jump  = 1) const;
  void DrawCircle(Double_t tiers_no,
                  Double_t tier,
                  Double_t collections_no,
                  Double_t collection,
                  Double_t pass_ratio,
                  Double_t radii,
                  Color_t passed_col,
                  Color_t failed_col,
                  TString label) const;
  void DrawLine(Double_t tiers_no,
                Double_t tier_from,
                Double_t tier_to,
                Double_t collections_from_no,
                Double_t collections_to_no,
                Double_t collection_from,
                Double_t collection_to,
                Double_t width,
                Int_t style,
                Color_t color) const;
  TString GetGroupListName(Hal::ECutUpdate update) const;
  HalHtmlFile* fHTML;
  TString fDir;
  Int_t fObjectCounter, fDynamicTableCounter, fTaskTableCounter;
  Int_t fCutContainerPosition, fPackageID;
  Int_t fCollectionsNo[4];
  Double_t fBasicRadius, fWindowWidth, fWIndowHeight;
  HalPackage* fCurrentCutContainer;
  Int_t fListDeep;
  Int_t fSoftVer;
  void TryExtractAnaResult(HalHtmlObject& table, HalPackage* pack, TString path);

protected:
  /**
   * default empty ctor
   */
  HalPackage2HTML();
  /**
   * constructor used by package extractor
   * @param ana_pack package with analysis
   * @param global_metadata package with global metadata
   * @param dir place where report will be created
   * @param task_id id of task
   */
  HalPackage2HTML(HalPackage* ana_pack, HalPackage* global_metadata, const TString dir, Int_t task_id);

public:
  /**
   * extract entire file to HTML report
   * @param filename name of file
   * @param dir place to create report
   */
  HalPackage2HTML(TString filename, const TString dir);
  /**
   * extract only single package into HTML report
   * @param filename name of file
   * @param dir place where report will be extracted
   * @param packname name of package to extraction
   */
  HalPackage2HTML(TString filename, const TString dir, TString packname);
  /**
   * export single package (even if it is not package with analysis
   * @param pack package for extraction
   * @param dir place where report is created
   * @param main_dir - if true separate coptions of html-scripts is created in
   * dir
   */
  HalPackage2HTML(HalPackage* pack, const TString dir, Bool_t mainDir = kTRUE);
  virtual ~HalPackage2HTML();
  ClassDef(HalPackage2HTML, 1)
};

#endif /* HALPACKAGE2HTML_H_ */
