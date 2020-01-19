# Physics

## PhysX    https://github.com/NVIDIAGameWorks/PhysX
### 必要なもの
- CMake
- Python
- DXSDK
    - インストール時s1023エラーで怒られる場合
		- 2010 再頒布可能パッケージを(2010 x86 Redistributable, 2010 x64 Redistributable)を一旦アンインストール
		- DXSDKをインストール
		- (古いのが入るので) 2010 再頒布可能パッケージをもう一度アンインストール
		- 最新の2010 再頒布可能パッケージをDLしてインストールする https://www.microsoft.com/ja-jp/download/confirmation.aspx?id=14632

### ビルド
-   physx/generate_projects.bat を実行する
    - ここではvc16win64(vs2019)を選択した
- physx/compipler/vc16win64/PhysXSDK.sln が作られるので開く
- Build - Batch Build - ALL_BUILD - checkedx64 にチェックを入れビルド
    -  typeinfo.h が無いと怒られる場合
        - PsAllocator.h
        ~~~
        //#include <typeinfo.h>
        #include <typeinfo>
        ~~~
    - SampleRendererでwarningが出てコンパイルが通らない場合
        - Property - C/C++ - General - Treat Warnings As Errors を Noにする

### 使用
#### VSの設定
- Property - C/C++ - Additional Include Directries 
~~~
PhysX\physx\include
PhysX\pxshared\include
~~~
- Property - Linker- Additional Library Directries
~~~
PhysX\physx\bin\win.x86_64.vc$(PlatformToolsetVersion).mt\checked
//$(PlatfromToolsetVersion) には例えば 142　が入っている
~~~
#### コード記述
~~~
#include "PxPhysicsAPI.h"
~~~
~~~
#pragma comment(lib, "PhysXCommon_64.lib")
#pragma comment(lib, "PhysX_64.lib")
#pragma comment(lib, "PhysXFoundation_64.lib")

//#pragma comment(lib, "PhysXCooking_64.lib")
//#pragma comment(lib, "PhysXGpu_64.lib")
~~~