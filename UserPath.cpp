#include "stdafx.h"

#include "UserPath.h"
#include <windows.h>
#include <userenv.h>
#include <shlobj.h>
 
// ------------------------------------------------------------------
// リンクするライブラリ
// ------------------------------------------------------------------
#pragma comment(lib, "userenv.lib")
 
// ------------------------------------------------------------------
// マクロ
// ------------------------------------------------------------------
/**
* @brief ディレクトリ操作メソッド用の最大パス文字数
*
* MAX_PATH は 260 バイト分しかないので、足りない場合に備えて
* 4096 バイトに拡張
*/
#define USERDIRECTORY_MAX_PATH (4096)
 
 
// ------------------------------------------------------------------
// プライベートメソッド
// ------------------------------------------------------------------
 
/**
* @brief Win32API のシェル関数を使ってディレクトリを取得
*
* @param nFolder CSIDL
* http://msdn.microsoft.com/en-us/library/bb762494(VS.85).aspx
* @return ディレクトリ名
* @note 失敗した時は空文字列を返す
*/
std::string UserPath::getShellDirectory(int nFolder)
{
HRESULT result;
LPITEMIDLIST pidl;
std::string str;
IMalloc *pMalloc;
 
SHGetMalloc(&pMalloc);
result = ::SHGetSpecialFolderLocation(NULL,
nFolder,
&pidl);
 
if (FAILED(result))
{
pMalloc->Release();
str.assign("\0", 1);
return str;
}
 
str.assign("\0", USERDIRECTORY_MAX_PATH + 1);
::SHGetPathFromIDList(pidl, (LPSTR)str.c_str());
 
pMalloc->Free(pidl);
pMalloc->Release();
 
return str;
}
 
 
// ------------------------------------------------------------------
// パブリックメソッド
// ------------------------------------------------------------------
 
/**
* @brief ユーザディレクトリ一覧を取得
*
* @code
* // コード例
* map<string, string> dirs;
* dirs = UserPath::map();
*
* // 1 項目分取得
* cout << dirs["UserProfiles"];
*
* // 項目全てをリストアップ
* for (map<string, string>::iterator i = dirs.begin(); i != dirs.end(); i++)
* {
*   pair<string, string> item = (*i);
*   cout << item.first << " => " << item.second << endl;
* }
* @endcode
*
* 取得可能なキーのリスト
*
* @li UserProfiles
* @li MyDocuments
* @li MyMusic
* @li MyVideos
* @li StartMenu
* @li ProgramMenu
* @li Startup
* @li ApplicationData
* @li Desktop
* @li SendTo
* @li Templates
* @li NetHood
* @li PrintHood
* @li Recent
* @li InternetCache
* @li Favorites
* @li History
* @li Cookies
* @li CDBurn
*
* @return ユーザディレクトリ一覧
*/
std::map<std::string, std::string> UserPath::map()
{
std::map<std::string, std::string> result;
 
result.insert(std::pair<std::string, std::string>("UserProfiles", userProfiles()));
result.insert(std::pair<std::string, std::string>("MyDocuments", myDocuments()));
result.insert(std::pair<std::string, std::string>("MyMusic", myMusic()));
result.insert(std::pair<std::string, std::string>("MyPictures", myPictures()));
result.insert(std::pair<std::string, std::string>("MyVideos", myVideos()));
result.insert(std::pair<std::string, std::string>("StartMenu", startMenu()));
result.insert(std::pair<std::string, std::string>("ProgramMenu", programMenu()));
result.insert(std::pair<std::string, std::string>("Startup", startup()));
result.insert(std::pair<std::string, std::string>("ApplicationData", applicationData()));
result.insert(std::pair<std::string, std::string>("Desktop", desktop()));
result.insert(std::pair<std::string, std::string>("SendTo", sendTo()));
result.insert(std::pair<std::string, std::string>("Templates", templates()));
result.insert(std::pair<std::string, std::string>("NetHood", netHood()));
result.insert(std::pair<std::string, std::string>("PrintHood", printHood()));
result.insert(std::pair<std::string, std::string>("Recent", recent()));
result.insert(std::pair<std::string, std::string>("InternetCache", internetCache()));
result.insert(std::pair<std::string, std::string>("Favorites", favorites()));
result.insert(std::pair<std::string, std::string>("History", history()));
result.insert(std::pair<std::string, std::string>("Cookies", cookies()));
result.insert(std::pair<std::string, std::string>("CDBurn", cdBurn()));
 
return result;
}
 
/**
* @brief Documents and Settings 内から、現在のユーザのディレクトリを
* フルパスで取得
*
* @code
* // 出力例 (終端に \ は含まない)
* C:\Documents and Settings\ユーザ名
* @endcode
*
* @return Documents and Settings 内の、現在のユーザのディレクトリ
* @note 失敗した時は空文字列を返す
*/
std::string UserPath::userProfiles()
{
BOOL result;
HANDLE processHandle;
HANDLE tokenHandle;
DWORD size;
std::string str;
 
processHandle = ::GetCurrentProcess();
result = ::OpenProcessToken(processHandle,
TOKEN_QUERY,
&tokenHandle);
if (result == 0)
{
str.assign("\0", 1);
return str;
}
 
 
str.assign("\0", USERDIRECTORY_MAX_PATH + 1);
size = USERDIRECTORY_MAX_PATH;
result = ::GetUserProfileDirectory(tokenHandle,
(LPSTR)str.c_str(),
&size);
 
if (result == 0)
str.assign("\0", 1);
 
return str;
}
 
/**
* @brief マイドキュメントのディレクトリをフルパスで取得
*
* @code
* // 出力例 (終端に \ は含まない)
* C:\Documents and Settings\ユーザ名\My Documents
* @endcode
*
* @return マイドキュメントのディレクトリ
* @note 失敗した時は空文字列を返す
*/
std::string UserPath::myDocuments()
{
return getShellDirectory(CSIDL_PERSONAL);
}
 
/**
* @brief マイミュージックのディレクトリをフルパスで取得
*
* @code
* // 出力例 (終端に \ は含まない)
* C:\Documents and Settings\ユーザ名\My Documents\My Music
* @endcode
*
* @return マイミュージックのディレクトリ
* @note 失敗した時は空文字列を返す
*/
std::string UserPath::myMusic()
{
return getShellDirectory(CSIDL_MYMUSIC);
}
 
/**
* @brief マイピクチャーのディレクトリをフルパスで取得
*
* @code
* // 出力例 (終端に \ は含まない)
* C:\Documents and Settings\ユーザ名\My Documents\My Pictures
* @endcode
*
* @attention IE5 以降または Windows 2000 以降
* @return マイピクチャーのディレクトリ
* @note 失敗した時は空文字列を返す
*/
std::string UserPath::myPictures()
{
return getShellDirectory(CSIDL_MYPICTURES);
}
 
/**
* @brief マイビデオのディレクトリをフルパスで取得
*
* @code
* // 出力例 (終端に \ は含まない)
* C:\Documents and Settings\ユーザ名\My Documents\My Videos
* @endcode
*
* @attention Windows XP 以降
* @return マイビデオのディレクトリ
* @note 失敗した時は空文字列を返す
*/
std::string UserPath::myVideos()
{
return getShellDirectory(CSIDL_MYVIDEO);
}
 
/**
* @brief スタートメニューのディレクトリを
* フルパスで取得
*
* @code
* // 出力例 (終端に \ は含まない)
* C:\Documents and Settings\ユーザ名\スタート メニュー
* @endcode
*
* @return スタートメニューのディレクトリ
* @note 失敗した時は空文字列を返す
*/
std::string UserPath::startMenu()
{
return getShellDirectory(CSIDL_STARTMENU);
}
 
/**
* @brief プログラムメニューのディレクトリを
* フルパスで取得
*
* @code
* // 出力例 (終端に \ は含まない)
* C:\Documents and Settings\ユーザ名\スタート メニュー\プログラム
* @endcode
*
* @return プログラムメニューのディレクトリ
* @note 失敗した時は空文字列を返す
*/
std::string UserPath::programMenu()
{
return getShellDirectory(CSIDL_PROGRAMS);
}
 
/**
* @brief スタートアップのディレクトリをフルパスで取得
*
* @code
* // 出力例 (終端に \ は含まない)
* C:\Documents and Settings\ユーザ名\スタート メニュー\プログラム\スタートアップ
* @endcode
*
* @return スタートアップのディレクトリ
* @note 失敗した時は空文字列を返す
*/
std::string UserPath::startup()
{
return getShellDirectory(CSIDL_STARTUP);
}
 
/**
* @brief アプリケーションデータのディレクトリを
* フルパスで取得
*
* @code
* // 出力例 (終端に \ は含まない)
* C:\Documents and Settings\ユーザ名\Application Data
* @endcode
*
* @return アプリケーションデータのディレクトリ
* @note 失敗した時は空文字列を返す
*/
std::string UserPath::applicationData()
{
return getShellDirectory(CSIDL_APPDATA);
}
 
/**
* @brief デスクトップのディレクトリをフルパスで取得
*
* @code
* // 出力例 (終端に \ は含まない)
* C:\Documents and Settings\ユーザ名\デスクトップ
* @endcode
*
* @return デスクトップのディレクトリ
* @note 失敗した時は空文字列を返す
*/
std::string UserPath::desktop()
{
return getShellDirectory(CSIDL_DESKTOPDIRECTORY);
}
 
/**
* @brief "送る" メニューのディレクトリをフルパスで取得
*
* ファイルを右クリックした時に出てくる、
* "送る" メニューのディレクトリを取得します。
*
* @code
* // 出力例 (終端に \ は含まない)
* C:\Documents and Settings\ユーザ名\SendTo
* @endcode
*
* @return "送る" メニューのディレクトリ
* @note 失敗した時は空文字列を返す
*/
std::string UserPath::sendTo()
{
return getShellDirectory(CSIDL_SENDTO);
}
 
/**
* @brief "新規作成" メニューのディレクトリをフルパスで取得
*
* ファイルを右クリックした時に出てくる、
* "新規作成" メニューのディレクトリを取得します。
*
* @code
* // 出力例 (終端に \ は含まない)
* C:\Documents and Settings\ユーザ名\Templates
* @endcode
*
* @return "新規作成" メニューのディレクトリ
* @note 失敗した時は空文字列を返す
*/
std::string UserPath::templates()
{
return getShellDirectory(CSIDL_TEMPLATES);
}
 
/**
* @brief NetHood のディレクトリをフルパスで取得
*
* NetHood は、他のコンピュータへのリンクが入ったディレクトリです。
* リンクは、 LAN 内の共有ディレクトリへの場所等を指します。
*
* @code
* // 出力例 (終端に \ は含まない)
* C:\Documents and Settings\ユーザ名\NetHood
* @endcode
*
* @return NetHood のディレクトリ
* @note 失敗した時は空文字列を返す
*/
std::string UserPath::netHood()
{
return getShellDirectory(CSIDL_NETHOOD);
}
 
/**
* @brief PrintHood のディレクトリをフルパスで取得
*
* PrintHood は、他のコンピュータへのリンクが入ったディレクトリです。
* リンクは、 LAN 内の共有プリンタへの場所を指します。
*
* @code
* // 出力例 (終端に \ は含まない)
* C:\Documents and Settings\ユーザ名\PrintHood
* @endcode
*
* @return PrintHood のディレクトリ
* @note 失敗した時は空文字列を返す
*/
std::string UserPath::printHood()
{
return getShellDirectory(CSIDL_PRINTHOOD);
}
 
/**
* @brief "最近使ったファイル" のディレクトリを
* フルパスで取得
*
* @code
* // 出力例 (終端に \ は含まない)
* C:\Documents and Settings\ユーザ名\Recent
* @endcode
*
* @return "最近使ったファイル" のディレクトリ
* @note 失敗した時は空文字列を返す
*/
std::string UserPath::recent()
{
return getShellDirectory(CSIDL_RECENT);
}
 
/**
* @brief IE のキャッシュのディレクトリをフルパスで取得
*
* @code
* // 出力例 (終端に \ は含まない)
* C:\Documents and Settings\ユーザ名\Local Settings\Temporary Internet Files
* @endcode
*
* @return IE のキャッシュのディレクトリ
* @note 失敗した時は空文字列を返す
*/
std::string UserPath::internetCache()
{
return getShellDirectory(CSIDL_INTERNET_CACHE);
}
 
/**
* @brief IE の "お気に入り" のディレクトリをフルパスで取得
*
* @code
* // 出力例 (終端に \ は含まない)
* C:\Documents and Settings\ユーザ名\Favorites
* @endcode
*
* @return IE の "お気に入り" のディレクトリ
* @note 失敗した時は空文字列を返す
*/
std::string UserPath::favorites()
{
return getShellDirectory(CSIDL_FAVORITES);
}
 
/**
* @brief IE の履歴のディレクトリをフルパスで取得
*
* @code
* // 出力例 (終端に \ は含まない)
* C:\Documents and Settings\ユーザ名\Local Settings\History
* @endcode
*
* @return IE の履歴のディレクトリ
* @note 失敗した時は空文字列を返す
*/
std::string UserPath::history()
{
return getShellDirectory(CSIDL_HISTORY);
}
 
/**
* @brief IE のクッキーのディレクトリをフルパスで取得
*
* @code
* // 出力例 (終端に \ は含まない)
* C:\Documents and Settings\ユーザ名\Cookies
* @endcode
*
* @return IE のクッキーのディレクトリ
* @note 失敗した時は空文字列を返す
*/
std::string UserPath::cookies()
{
return getShellDirectory(CSIDL_COOKIES);
}
 
/**
* @brief OS の CD 書き込み機能で使うディレクトリをフルパスで取得
*
* @code
* // 出力例 (終端に \ は含まない)
* C:\Documents and Settings\ユーザ名\Local Settings\Application Data\Microsoft\CD Burning
* @endcode
*
* @attention Windows XP 以降
* @return OS の CD 書き込み機能で使うディレクトリ
* @note 失敗した時は空文字列を返す
*/
std::string UserPath::cdBurn()
{
return getShellDirectory(CSIDL_CDBURN_AREA);
}