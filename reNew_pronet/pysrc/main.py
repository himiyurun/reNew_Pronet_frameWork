import glbs

def print_hi(name):
    # スクリプトをデバッグするには以下のコード行でブレークポイントを使用してください。
    print(f'Hi, {name}')  # Ctrl+F8を押すとブレークポイントを切り替えます。

def main():
    num = 0
    for i in range(16):
        text = ''
        for j in range(16):
            if (j + (i % 16)) % 2 == 0:
                text += '#'
            else:
                text += ' '
        print(text)
    print_hi('Python')

ct = 0
def sample():
    global ct
    ct += 1
# ガター内の緑色のボタンを押すとスクリプトを実行します。
if __name__ == '__main__':
    print_hi('PyCharm')
main()
glbs.Loop(sample, 10000000)
print(ct)
for i in range(10000000):
    ct += 1
print(ct)

