# これはサンプルの Python スクリプトです。

# Shift+F10 を押して実行するか、ご自身のコードに置き換えてください。
# Shift を2回押す を押すと、クラス/ファイル/ツールウィンドウ/アクション/設定を検索します。


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
                text += ' 'tiyii
        print(text)
    print_hi('Python')
# ガター内の緑色のボタンを押すとスクリプトを実行します。
if __name__ == '__main__':
    print_hi('PyCharm')
main()

# PyCharm のヘルプは https://www.jetbrains.com/help/pycharm/ を参照してください
