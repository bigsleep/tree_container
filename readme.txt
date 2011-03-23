1. 概要

これはC++用の木構造コンテナライブラリです。
このライブラリにはtemplate class creek::tree<T, A>及び, template class creek::nary_tree<N, T, A>が含まれています.
ドキュメントはまだ書かれていませんが、概要を書いておきます.

1.1 Creek.Tree
template class creek::tree<T, A>は汎用な木構造コンテナです.
templateパラメータTはコンテナに収納される要素の型を, Aはアロケーターの型を表します.
Aはデフォルトではstd::allocator<T>となります.
このコンテナではノードの数や深さには制限がありません.
様々な走査方法が提供されています.
preorder_iterator, postorder_iterator, child_iterator及びこれらのconstバージョン, reverseバージョン, const reverseバージョンのiteratorが提供されています。
iteratorを用いて位置を指定してノードの追加や削除が行えます.

1.2 Creek.Nary_Tree
template class creek::nary_tree<N, T, A>はノードの数を固定したN分木構造のコンテナです.
templateパラメータNはstd::size_t型の定数パラメータで一つのノードが持つ子ノードの数を表します.
templateパラメータTはコンテナに収納されるオブジェクトの型を, Aはアロケーターの型を表します.
Aはデフォルトではstd::allocator<T>となります.
このクラスはstd::vector<T>を使って、レベル順にオブジェクトを確保しています.
このためコンテナが収納する要素に対してランダムアクセスすることができます.
ただし構築時にノードの深さを指定する必要があります.
コンテナの走査方法としてはレベル順のiterator及びこのconstバージョン, reverseバージョン, const reverseバージョンが提供されます.

------------------------------------------------------------------------------------------

2. ビルドの方法

ビルドにpythonで書かれたビルドシステムであるwafを使用しています。
このためpythonが必要です。
ライブラリのテストにはBoost.Testを使用しているため、テストをビルドするには
Boostが必要です。

2.1 ライブラリのみをビルドする場合
$ ./waf configure
$ ./waf build

2.2 ライブラリとexampleをビルドする場合
$ ./waf configure
$ ./waf example build

2.3 ライブラリとtestをビルドする場合
$ ./waf configure
$ ./waf test build

2.4 ライブラリとexampleとtestをビルドする場合
$ ./waf configure
$ ./waf example test build

