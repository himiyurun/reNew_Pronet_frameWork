from contextlib import nullcontext
import math
import glbs



def setup():
    # py::object _exec　: アプリケーションのループ中に実行する関数（現在は固定の関数が実行される、コールバック方式ではなく動的ロード方式に変更されているため未サポート）
    # py::object _gen　: 弾幕の生成時に実行する関数（コールバック方式ではなく動的ロード方式に変更されているため未サポート、適当な関数を入れても問題ないが引数をとる必要がある）
    # py::object _upd : 弾幕の更新時に実行される関数（コールバック方式ではなく動的ロード方式に変更されているため未サポート、適当な関数を入れても問題ないが引数をとる必要がある）
    # float _coef : 係数
    # float _area : 弾幕の有効な範囲（半径）
    # size_t _count : 弾幕の発射する個数
    # uint32_t _interval : 弾幕の発射される間隔
    # float _rad : 弾幕の発射する角度
    # float _speed : 弾幕の速度
    glbs.createPythonSetupInfo(exec, generate, update, 1.0, 16.0, 12, 10, 360.0, 0.007)
    return nullcontext

def exec():
    print('Execute')
    return nullcontext

# 弾幕の生成時に実行される関数
# posX: 座標、現状、プレイヤーの位置が渡される
# posY: 座標、現状、プレイヤーの位置が渡される
# status: キーボードの判定（スペースキー）
# param_data: 弾幕のデータ
# beg_posX: 弾幕の原点（始点）
# beg_posY: 弾幕の原点（始点）
# beg_rad: 弾幕の始める角度
# radiculous: 弾幕の初期の半径
# script: スクリプトの番号（無視してかまわない）
# obj_data: 新たに作成された弾幕オブジェクトのデータ（参照型のため変更が反映される）
def generate(posX, posY, status, param_data, beg_posX, beg_posY, beg_rad, radiculous, script, obj_data):
    obj_data.posX = posX
    obj_data.posY = posY
    obj_data.begDeg = 0.0
    obj_data.rad = 0.0
    return nullcontext

# 弾幕の更新時に実行される関数
# pos: 座標、バインドされた敵の位置が渡される
# status: 当たり判定
# param_data: 弾幕のデータ
# obj_data: 1弾幕のデータ（参照型のため変更が反映される）
# script: パイソンスクリプトの番号
# index: 弾幕オブジェクト配列のインデックス
# obj_param: その弾幕オブジェクトのパラメーター（参照型のため変更が反映される）
def update(posX, posY, status, param_data, obj_data, script, index, obj_param):
    bullet_rad = glbs.toRad(360.0) / param_data.count * index
    obj_data.rad += param_data.speed
    obj_param.posX = math.cos(obj_param.deg) * obj_data.rad
    obj_param.posY = math.sin(obj_param.deg) * obj_data.rad
    return nullcontext