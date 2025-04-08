from contextlib import nullcontext
from pyglm import vec4, vec3, vec2

def execute(_posX, _posY, _gen_stat):
    _posX = 0.0
    _posY = 0.0
    return nullcontext

def generate(_pos, _beg_rad, _radius, _bullets):
    print('generate')
    return nullcontext

def update(_pos, _beg_rad, _radius, _bullets):
    print('update')
    return nullcontext

def test(_n):
    _n += 1
    return nullcontext

numb = 0
for i in range(10):
    test(numb)
print(numb)