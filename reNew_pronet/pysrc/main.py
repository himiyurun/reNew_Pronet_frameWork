from contextlib import nullcontext
import sys
sys.path.append("/Users/A20046/source/repos/himiyurun/reNew_Pronet_frameWork/x64/Release")

import glbs

def exec(data, obj):
    glbs.printBulletParamData(data)
    print('Execute')
    return nullcontext

# 弾幕の生成時に実行される関数
# pos: 座標、バインドされた敵の位置が渡される
# status: 当たり判定
# param_data: 弾幕のデータ
# beg_pos: 弾幕の原点（始点）
# beg_rad: 弾幕の始める角度
# radiculous: 弾幕の初期の半径
# script: スクリプトの番号
# obj_data: 新たに作成された弾幕オブジェクトのデータ
def generate(posX, posY, status, param_data, beg_posX, beg_posY, beg_rad, radiculous, script, obj_data):
    print('Generate')
    return nullcontext

# 弾幕の更新時に実行される関数
# pos: 座標、バインドされた敵の位置が渡される
# status: 当たり判定
# param_data: 弾幕のデータ
# obj_data: 1弾幕のデータ
# script: パイソンスクリプトの番号
# index: 弾幕オブジェクトのインデックスが返される
# obj_param: その弾幕オブジェクトのパラメーター
def update(posX, posY, status, param_data, obj_data, script, index, obj_param):
    print('Update')
    return nullcontext

# py::object _exec
# py::object _gen
# py::object _upd
# float _coef
# float _area
# size_t _count
# uint32_t _interval
# float _rad
# float _speed
print('Hello World')
glbs.createPythonSetupInfo(exec, generate, update, 1.0, 12.0, 12, 10, 2.5, 0.07)
a = glbs.getLastClock()
print(a)
print('Hello World')
pos = [0.0, 0.0]
#glbs.test(pos)
b = glbs.BulletParamData()
c = glbs.BulletObjectData()
exec(b, c)
glbs.createBulletObject()