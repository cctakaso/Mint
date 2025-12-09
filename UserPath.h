/**
* @brief 現在のユーザに関連付けられたディレクトリを取得するクラス
* @file UserPath.h
* @date 2008/03/10
*/
 
#pragma once
#include <string>
#include <map>
 
/**
* @brief 現在のユーザに関連付けられたディレクトリを取得するクラス
* @code
* インスタンス化しないで、静的に使用する
*
* 例 1 :
* // マイドキュメントディレクトリを取得
* std::string myDoc = UserPath::myDocuments();
*
* 例 2 :
* // キーと値のリスト形式で取得
* map<string, string> dirs = UserPath::map();
* @endcode
*/
class UserPath
{
public:
static std::map<std::string, std::string> map();
 
static std::string userProfiles();
 
static std::string myDocuments();
static std::string myMusic();
static std::string myPictures();
static std::string myVideos();
 
static std::string startMenu();
static std::string programMenu();
static std::string startup();
 
static std::string applicationData();
static std::string desktop();
static std::string sendTo();
static std::string templates();
static std::string netHood();
static std::string printHood();
static std::string recent();
 
static std::string internetCache();
static std::string favorites();
static std::string history();
static std::string cookies();
static std::string cdBurn();
 
private:
static std::string getShellDirectory(int nFolder);
};
