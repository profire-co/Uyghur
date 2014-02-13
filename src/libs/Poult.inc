<?php
/*
 * Poult
 * Copyright (c) 2014 Profire Co.,Ltd
 *
 * This software is released under the MIT License.
 *
 * http://opensource.org/licenses/mit-license.php
 */
namespace pml\alone;
//  useしない

//****************************************************************************
/**
*   テンプレートを処理して目的の出力（HTMLなど）を取得します。
*
*   　PoultはPmlTemplateを単体利用用にPmlから切り出したものです。[/]
*
*   @constant LF
*       改行コード(LF:0x0A)が定義されています。
*
*   @constant ENCODING_CODE
*       本ライブラリのプログラムコードのエンコーディングが定義されています。
*
*   @constant ENCODING_WINFS
*       ウィンドウズのファイル名のエンコーディングが定義されています。
*
*   @constant REGEXP
*       タグ動作を検出するための内部処理用の正規表現が定義されています。
*
*   @constant REGION_PARENT
*       ディレクトリトラバースのモードを親テンプレート以下（兄弟のみ）に設定する内部処理用の文字列が定義されています。
*
*   @constant REGION_ROOT
*       ディレクトリトラバースのモードを開始テンプレート以下（家族全体）に設定する内部処理用の文字列が定義されています。
*/
//****************************************************************************
class Poult
{
    const LF       ="\n";
    const ENCODING_CODE = 'UTF-8';  //  このファイルの encoding(mb_detect_encoding()の結果と比較)
    const ENCODING_WINFS= 'CP932';  //  Windows File System の encoding

    //========================================================================
    //  properties
    //========================================================================
    /** テンプレートのコンパイル結果を格納するディレクトリを示します。  */
    protected $compile_dir;

    /** 親テンプレートのオブジェクトを格納しています（ルートのときはNULL）。 */
    protected $parent;

    /** このテンプレートが読み込まれたネストの深さを格納しています（ルートの時は0）。       */
    protected $depth=0;

    /** fetch()に指定されたときの正規化されていないリソース名（主に相対パス）を格納しています。   */
    protected $org;

    /** テンプレート処理を行うための正規化されたリソース名（主にフルパス）を格納しています。  */
    protected $dsn;

    /** テンプレートにアサインされた変数群（連想配列）を格納しています。    */
    protected $assigns;

    /** 強制コンパイルをするかどうか？を決める二値を保存しています。            */
    protected $force_compile;

    //  works for compile
    /** PHPにコンパイルされたテンプレートのファイル名を格納しています。            */
    protected $cache_filename;

    /** captureタグを利用した時にキャプチャ内容を一時的に保存しています。   */
    protected $capture_vars=array();

    /** テンプレートで指定されたネームスペースを保存しています。        */
    protected $namespace;

    /** 開きタグ文字列を格納しています。    */
    private $tag_open;

    /** 閉じタグ文字列を格納しています。    */
    private $tag_close;

    /** dsn解決時やコンパイル済みテンプレート出力時のエラーメッセージ設定を格納しています。   */
    private $message;

    /** テンプレートを解釈せずにそのまま出力するxmpタグのオープン数を管理する内部変数です。  */
    private $xmp;

    /** インポート時のディレクトリトラバースチェックの方式を REGION_ROOTかREGION_PARENTで保存しています。 */
    private $region;

    const REGION_ROOT   ='root';
    const REGION_PARENT ='parent';

    //========================================================================
    /**
    *   コンストラクタ
    *
    *   @param compile_dir  コンパイル済みテンプレート(phpファイル)の保存先
    *   @param parent       親テンプレート
    *   @param option       オプション
    */
    //========================================================================
    public function __construct($compile_dir=NULL,$parent=NULL,$option=NULL)
    {
        //  内部パラメータ初期化
        $this->parent   = $parent;
        $this->depth    = $parent ? $parent->depth+1 : 0;
        $this->region   = self::REGION_ROOT;
        //  優先順位 : 引数 > 親 > __DIR__
        $this->compile_dir  = $compile_dir ?: ($parent ? $parent->compile_dir : __DIR__);
    }

    //========================================================================
    /**
    *   import時のディレクトリトラバースのモードを親テンプレート以下に設定します。
    *
    *   デフォルトは、ルートテンプレート以下に設定されており、より厳しい親テンプレート以下に設定することしかできません。
    */
    //========================================================================
    public function setRegionParent(){
        $this->region = self::REGION_PARENT;
    }

    //************************************************************************
    //  ファイルシステム
    //************************************************************************
    private static function path_encode($path){
        if ( PHP_OS=='WINNT' ){
            return mb_convert_encoding($path,self::ENCODING_WINFS,self::ENCODING_CODE);
        }
        return $path;
    }

    //========================================================================
    /**
    *   ファイルのタイムスタンプを取得します（Windows/マルチバイト対応）
    */
    //========================================================================
    protected static function file_mtime($path){
        return filemtime(self::path_encode($path));
    }

    //========================================================================
    /**
    *   ファイルの内容を取得します（Windows/マルチバイト対応）
    */
    //========================================================================
    protected static function file_read($path){
        return file_get_contents(self::path_encode($path));
    }

    //========================================================================
    /**
    *   ファイルにデータを書きだします（Windows/マルチバイト対応）
    */
    //========================================================================
    protected static function file_write($path,$contents){
        return file_put_contents(self::path_encode($path),$contents);
    }

    //========================================================================
    /**
    *   ファイルの存在を調べずにパスを正規化します。
    *
    *   '\' ->'/'
    *   '/./'  ->'/'
    *   '/../' ->一つ上のディレクトリを削除
    */
    //========================================================================
    protected static function path_normalize($path)
    {
        if ( !$path ){
            return '';
        }
        //  '\' を '/' にを正規化
        $path = str_replace('\\','/',$path);

        //  末尾が'.'か'..'のときは'/'を付与する（変換のために）
        if ( preg_match('/\.?\.$/',$path) ){
            $path .= '/';
        }

        //  '/./' を '/' に正規化
        $path = preg_replace('#/\./#','/',$path);

        //  '/../' が存在しなければ終了(preg_matchを廃止してstrstrに)
        if ( strstr($path,'/../')===FALSE ){
            return $path;
        }

        //  '/'でパスを分割する
        $path = explode('/',$path);

        //  パスのテーブルを準備
        $normal = array();
        foreach( $path as $name ){
            if ( $name=='..' ){
                if ( count($normal)==0 ){
                    return FALSE;          //  エラー
                }
                array_pop($normal);
            }
            else{
                $normal[] = $name;
            }
        }
        return implode('/',$normal);
    }

    //************************************************************************
    /**
    *   プロパティ参照
    */
    //************************************************************************
    /** ルートのテンプレートオブジェクトを返します。*/
    public function getRoot()
    {
        for( $parent=$this ; $parent->parent && $parent->region==self::REGION_ROOT; $parent=$parent->parent );
        return $parent;
    }

    /** 親のテンプレートオブジェクトを返します。*/
    public function getParent()
    {
        return $this->parent;
    }

    /** 正規化済みのデータソース名（殆どの場合ファイル名）を返します。 */
    public function getDSN()
    {
        return $this->dsn;
    }

    /** キャッシュファイル名（コンパイル済みテンプレートファイル名）を返します。  */
    public function getCacheFilename()
    {
        return $this->cache_filename;
    }

    /** テンプレートがインポートされている現在の深度を返します（ルートは０）。 */
    public function getDepth()
    {
        return $this->depth;
    }

    //************************************************************************
    //
    //  イベント（派生してカスタマイズ）
    //
    //************************************************************************
    //========================================================================
    /**
    *   テンプレートにアサインするパラメータの正規化を行います。デフォルトでは何もしません。
    *
    */
    //========================================================================
    protected function onNormalizeAssigns($assigns)
    {
        return $assigns;
    }

    //========================================================================
    /**
    *   データソース名を解決して onGetContents() の引数に渡せるようにします。
    *
    *   標準ではルートファイルの配下でないファイルにはアクセス出来ないが、
    *   派生すればその制限はない。
    */
    //========================================================================
    protected function onResolveDSN($dsn)
    {
        //
        //  パスの正規化
        //
        $dsn = str_replace('\\','/',$dsn);

        //
        //  $dsnが相対パス指定だったら 親($this->parent)dsnからの相対にする
        //  ドライブレター付きのルートディレクトリ表記に対応
        //
        if ( !preg_match('#^([a-zA-Z]:)?/#',$dsn) ){
            //  パスを取得して結合
            if ( $this->parent ){
                $dsn = dirname($this->parent->dsn).'/'.$dsn;
            }
            else{
                return FALSE;
            }
        }
        
        //  パスの比較のために'./' や '../' を正規化しておく
        $dsn = self::path_normalize($dsn);

        //  子のテンプレートを取得する場合は
        //  ルートテンプレートの配下でないファイルにはアクセス出来ない
        //  ※正規化（path_normalize()）されているので単純に比較できる
        if ( $this->depth>0 ){
            $dir = $this->region==self::REGION_ROOT ? $this->getRoot() : $this->getParent();
            $dir = dirname($dir->getDsn());
            //var_dump('DIR: '.$dir);
            //var_dump('DSN: '.$dsn);
            if ( strncmp($dir.'/',$dsn,strlen($dir)+1)!==0 ){
                return FALSE;
            }
        }

        return $dsn;
    }

    //========================================================================
    /**
     *  引数で示されたdsnが最後に更新された日時を返します。
     *  存在しないときはfalseを返します。
     */
    //========================================================================
    protected function onGetTimestamp($dsn)
    {
        return @self::file_mtime($dsn);
    }

    //========================================================================
    /**
     *  dsnからキャッシュファイル名（主にフルパス）を生成します。
     */
    //========================================================================
    protected function onGetCacheFilename($dsn)
    {
        //  パス名とクラス名でcrc32を採りHEX表記して新しいパスとする
        //  クラス名もつけないと派生クラスで同じテンプレートを参照してしまう
        $path = sprintf('%08X',crc32(dirname($dsn).get_class($this)));
        //    basename() / pathinfoがバグるゆえ
        $array=explode('/',str_replace('\\','/',$dsn));
        $basename = end($array);
        //return $path.'.'.basename($dsn).'.php';   //  まだUTF-8
        return $path.'.'.$basename.'.php';
    }

    //========================================================================
    /**
    *   dsnで示されるコンテンツを取得します。
    */
    //========================================================================
    protected function onGetContents($dsn)
    {
        return @self::file_read($dsn);
    }

    //========================================================================
    /**
    *   出力する直前のデータに任意の処理を実行します。デフォルトでは何もしません。
    */
    //========================================================================
    protected function onOutputFilter($contents)
    {
        return $contents;
    }

    //************************************************************************
    //
    //  テンプレート処理の主体
    //
    //************************************************************************
    //========================================================================
    /**
     *  
     */
    //========================================================================
    const WHEN_RESOLVE_DSN   = 'whenResolveDSN';
    const WHEN_GET_TIMESTAMP = 'whenGetTimestamp';
    const WHEN_GET_CONTENTS  = 'whenGetContents';

    protected function onError($error_code,$dsn,$normalized_name)
    {
        switch($error_code){
            case self::WHEN_RESOLVE_DSN:
                return '!allowed';
            case self::WHEN_GET_TIMESTAMP:
            case self::WHEN_GET_CONTENTS:
                return '!found';
        }
        return 'onError(): unknown';
    }

    //========================================================================
    /**
    *   テンプレートを処理して実行結果を返します。
    *
    */
    //========================================================================
    public function fetch($dsn,array $assigns,$force_compile=FALSE,&$is_break=NULL)
    {

        //--------------------------------------------------------------------
        //  フィルタなどを通す前にクラスに保存
        //--------------------------------------------------------------------
        $this->assigns        = $assigns;
        $this->force_compile    = $force_compile;

        //--------------------------------------------------------------------
        //  dsnを正規化（解決）し、アクセス許可なども調べる
        //  ここではリソースが実在するかどうかは関知しない
        //--------------------------------------------------------------------
        $this->org = $dsn;
        $this->dsn = $this->onResolveDSN($this->org);
        if ( $this->dsn===FALSE ){
            //  コールバックして例外投げるかどうかはそこで決めさす
            $is_break = FALSE;
            return $this->onError(self::WHEN_RESOLVE_DSN,$dsn,NULL);
        }

        //--------------------------------------------------------------------
        //  キャッシュファイル名を取得してタイムスタンプを取得
        //--------------------------------------------------------------------
        $this->cache_filename = $this->compile_dir . '/' . $this->onGetCacheFilename($this->dsn);
        $cache_timestamp = @self::file_mtime($this->cache_filename);

        //--------------------------------------------------------------------
        //  対象dsnのタイムスタンプを取得（リソースが存在するかどうかも確認）
        //--------------------------------------------------------------------
        $source_timestamp = $this->onGetTimestamp($this->dsn);
        if ( $source_timestamp===FALSE ){
            //  タイムスタンプを取得できなかったときはdsnが存在しないと判断
            $is_break = FALSE;
            return $this->onError(self::WHEN_GET_TIMESTAMP,$this->org,$this->dsn);
        }

        //--------------------------------------------------------------------
        //  コンパイル条件を判定してコンパイルしてキャッシュに残す
        //--------------------------------------------------------------------
        $source=FALSE;
        if ( $cache_timestamp===FALSE || $cache_timestamp<$source_timestamp || $force_compile ){

            //  dsnからソースを取得
            $source = $this->onGetContents($this->dsn);
            if ( $source===FALSE ){
                //  実際に取得できなかったときも存在しないと判断する
                $is_break = FALSE;
                return $this->onError(self::WHEN_GET_CONTENTS,$this->org,$this->dsn);
            }

            //  dnsソースのエンコーディングから内部エンコーディングに変換
            //  mb_detect_encoding()とかすると不安定になる？
            $native_encoding = mb_detect_encoding($source,'utf-8,sjis,eucjp');

            //UTF-8に変換
            if ( $native_encoding!=self::ENCODING_CODE ){
                $source = mb_convert_encoding($source,self::ENCODING_CODE,$native_encoding);
            }

            //  コンパイルと時間計測
            $begin = microtime(TRUE)*1000;
            $source = $this->onCompile($source);
            $time = round(microtime(TRUE)*1000 - $begin,2);

            //  コンパイル時間と元エンコーディングをテンプレートに保存しておく
            //  またインポート時間を保存する変数を初期化しておく
            $namespace = $this->namespace ? ('namespace '.$this->namespace.';') : '/* Here is global namespace. */';
            $header = <<< HERE
<?php
    $namespace
    \$__NATIVE_ENCODING__   ='{$native_encoding}';
    \$__COMPILE_TIME__    ={$time};
    \$__IMPORT_TIME__      =null;
?>\n
HERE;
            //  キャッシュとして保存（UTF-8)で保存
            self::file_write($this->cache_filename,$header.$source);
        }

        //--------------------------------------------------------------------
        //  フェッチ時に毎回通る前処理
        //--------------------------------------------------------------------
        $this->assigns = $this->onNormalizeAssigns($this->assigns);

        //  キャッシュを読み込んでドキュメント生成
        list($result,$is_break,$native_encoding) = $this->execute($this->cache_filename);

        //  元エンコーディングに戻す
        if ( $this->depth==0 ){
            if ( $native_encoding!='ASCII' && $native_encoding!='UTF-8'){
                $result = mb_convert_encoding($result,$native_encoding,self::ENCODING_CODE);
            }
        }

        //  出力前フィルター
        //  携帯の絵文字に関する処理はここでやるのが良いかもしれない
        //  HTMLエンティティ表記するなら気にしなくてもイイけど変換とかするなら
        $result = $this->onOutputFilter($result);
        //var_dump($result);

        return $result;
    }

    //========================================================================
    /**
    *   evalしようと思ったけどAPCで問題出るとかなのでrequire()する
    *   extract()＋require() のため、関数化してこのスコープに局所化した。
    */
    //========================================================================
    private function execute($filename)
    {
        //  extract - このスコープ内に変数を展開
        if ( is_array($this->assigns) ){
            extract($this->assigns);
        }

        // このスコープ内でキャッシュを実行(require)
        // $quit_codeは、$filname内でreturn FALSEが明示されなかったらTRUEが返る
        // {quitstop}時にはfalseが返る。
        ob_start();
        $quit_code = require self::path_encode($filename);
        $result = ob_get_contents();
        ob_end_clean();

        return array($result,!$quit_code,$__NATIVE_ENCODING__);
    }

    //************************************************************************
    /**
    *   コンパイル後のテンプレートから利用されるヘルパー
    */
    //************************************************************************
    //========================================================================
    /**
    *   executeの中のテンプレートの中からコールされるimport関数
    *   別のドキュメントを読み込んで表示
    *   {import 'class'=>{クラス名}}
    */
    //========================================================================
    private function import($args)
    {
        //  var_dump($args);

        //  時間計測開始
        $start_time = microtime(TRUE)*1000;

        //  newするクラス名を取得（引数に指定されなかった場合は親から継承）
        $args['class'] = (isset($args['class']) && $args['class']) ? $args['class'] : get_class($this);

        //  サブドキュメントクラス生成（基本的に親クラスから継承）
        $child_template = new $args['class'](NULL,$this,$args);
        if ( isset($args['region']) && $args['region']!='root' ){
            $child_template->setRegionParent();
        }

        //  コンパイルフラグも親から継承
        $force_compile  = isset($args['force_compile']) ? $args['force_compile']: $this->force_compile;

        //  親の引数を全て受け継ぎ、import関数の引数と併せてassign
        $all = array_merge($this->assigns,$args);

        //  ドキュメントを取り出す
        $result = $child_template->fetch($args['dsn'],$all,$force_compile,$is_break);
        echo $result;

        //  時間計測
        $import_time=round( microtime(TRUE)*1000 - $start_time,2 );

        //  ドキュメント終了コードを取得
        return array($is_break,$import_time,$child_template);
    }

    //************************************************************************
    /**
    *   ユーティリティ
    */
    //************************************************************************
    //========================================================================
    /**
    *   テンプレートを処理します。
    */
    //========================================================================
    public static function convert(
        $dsn,                       //  テンプレートファイル
        $assigns        =array(),   //  アサインする変数
        $strict         =TRUE,      //  fetch()に失敗したらエラー表示する？
        $compile_dir    =NULL,      //  コンパイルディレクトリ
        $force_compile  =false,     //  強制コンパイルするか？
        $parent         =NULL       //  親クラス
    )
    {
        //  テンプレート初期化
        $tmpl = new static($compile_dir,$parent);

        //  ドキュメント生成（コンストラクタで親ドキュメントを指定しない代わりにここはフルパスで）
        $result = $tmpl->fetch($dsn,$assigns,$force_compile,$is_break);

        //  ドキュメント表示
        return $result;
    }

    //========================================================================
    /**
    *   現在のクラス情報をもとにテンプレートを処理します。
    */
    //========================================================================
    function convert_child($dsn,$assigns,$strict=TRUE,$class=null)
    {
        //  クラスが指定されていなければ、$thisのクラスを引き継ぐ
        if ( !$class ){
            $class = get_class($this);
        }

        //  クラス
        return $class::convert(
            $dsn,
            $assigns,
            $strict,
            $this->compile_dir,
            $this->force_compile,
            $this
        );
    }

    //************************************************************************
    /**
    *   コンパイル時に利用される関数群
    */
    //************************************************************************
    //========================================================================
    /**
    *   タグを解釈してテンプレートをコンパイルします。
    */
    //========================================================================
    protected function onCompile($source)
    {
        $entities = function($string){
            $result='';
            for($i=0;$i<strlen($string);$i++){
                $result .= ('&#'.ord($string[$i]).';');
            }
            return $result;
        };

        //  タグを解析するよ
        $this->setTag('{:',':}');

        //  エスケープ表現 を HTMLEntities に変換
        $source = str_replace(
            //array('\\\\','\\{','\\}','\\&','\<','\>'),
            //array('&#x5c;','&#x7b;','&#x7d;','&amp;','&lt;','&gt;'),
            array('\\\\'    ,'\\&'  ,'\<'   ,'\>'    ,'\\'.$this->tag_open,'\\'.$this->tag_close),
            array('&#x5c;'  ,'&amp;','&lt;' ,'&gt;'  ,$entities($this->tag_open),$entities($this->tag_close)),
            $source);
        //var_dump($source);
        //$source = str_replace(['&{;','&};'],['{','}'],$source);

        //  改行コードを正規化
        $source = str_replace("\r\n",self::LF,$source);

        //$this->setTag('{','}');

        $source = $this->walk($source);

        //  HTMLEntitiesのうち {} を戻す
        //$source = str_replace(['&#x7b;','&#x7d;'],['{','}'],$source);
        //$source = str_replace(['&#x5c;','&#x7b;','&#x7d;'],['\\\\','{','}'],$text);
        $source = str_replace(array('&#x5c;'),array('\\'),$source);

        return $source;
    }

    /**
     *
     *    構文解析用のタグを設定します。デフォルトは、"{:",":}"です。
     *
     */
    protected function setTag($open,$close){
        $this->tag_open =$open;
        $this->tag_close=$close;
    }

    //========================================================================
    /**
    *   テキストを往来してタグを検出し、onOpen/onCloseを呼び出します。
    */
    //========================================================================
    protected function walk($source,$pos=0,$indent=0,$open_lf=null)
    {
        $text='';
        while($tag_info=$this->findTag($source,$pos)){
            list($is_open,$tag_pos,$lf)=$tag_info;

            //  タグより前を取得
            $text.= substr($source,$pos,$tag_pos-$pos);

            //  タグをスキップ  改行コードはonClose()で扱いを決める
            $pos = $tag_pos;
            $pos += ($is_open?strlen($this->tag_open):strlen($this->tag_close));
            $pos += ($lf ? 1: 0);

            //  タグに応じてコールバックしたり
            if ( $is_open ){
                $this->onOpen($source,$indent+1,$lf,$pos);
                list ($pos,$substr) = $this->walk($source,$pos,$indent+1,$lf);
                $text.=$substr;
            }
            else{
                $text = $this->onClose($text,$indent-1,$lf,$open_lf);
                //var_dump(bin2hex($text));
                break;                        //  閉じタグを見つけたら一回戻す
            }
        }

        if ( $indent==0 ){
            //  最後の閉じタグが終わってから末尾までのテキストを取得する
            return $text.substr($source,$pos);

        }
        return array($pos,$text);
    }

    //========================================================================
    /**
    *   テキストからタグ文字列を探し出します。
    */
    //========================================================================
    public function findTag($source,$pos)
    {
        $open_pos   = strpos($source,$this->tag_open ,$pos);
        $close_pos  = strpos($source,$this->tag_close,$pos);

        //--------------------------------------------------------------------
        //  タグの次の文字が改行かどうかを調べる
        //--------------------------------------------------------------------
        if ( $open_pos===FALSE ){
            if ( $close_pos===FALSE ){
                //  この先にタグは現れない
                $is_open = null;
                $tag_pos = -1;
            }
            else{
                $is_open = false;
                $tag_pos = $close_pos;
            }
        }
        else{
            if ( $close_pos===FALSE ){
                $is_open = true;
                $tag_pos = $open_pos;
            }
            else{
                //  先に現れた方が有効
                if ( $open_pos < $close_pos ){
                    $is_open = true;
                    $tag_pos = $open_pos;
                }
                else{
                    $is_open = false;
                    $tag_pos = $close_pos;
                }
            }
        }

        //--------------------------------------------------------------------
        //  タグの次の文字が改行かどうかを調べる
        //  null / false / self::LF のどれかが返る
        //--------------------------------------------------------------------
        $find = null;
        if ( ($is_open!==null) ){
            if ( $tag_pos < strlen($source)-1 ){
                $lf = ($source[$tag_pos+strlen($this->tag_close)]==self::LF) ? self::LF : null;
                //  phpの閉じタグが続く改行を無視してしまうので
                $find = array($is_open,$tag_pos,$lf);
            }
            else{
                $find = array($is_open,$tag_pos,false);
            }
        }
        return $find;
    }

    //========================================================================
    /**
    *   開きタグが見つかった時の処理を行います。
    */
    //========================================================================
    public function onOpen($text,$indent,$lf,$pos)
    {
        if ( substr($text,$pos,3)=='xmp' ){
            $this->xmp++;
        }
        // do nothing
    }

    const REGEXP='#^(xmp|if|while|foreach|for|else\s*if|elsif|import|code|=|namespace|capture|html_radios|html_options)\s#s';
    //========================================================================
    /**
    *   閉じるタグが見つかった時の処理を行います。
    *
    *   タグを閉じるときに、タグの内容をコールバックする
    *   コンパイルは出現頻度の高いものからチェックする
    */
    //========================================================================
    public function onClose($text,$indent,$lf,$open_lf)
    {
        //  XMPが開いてる時はタグのコンパイルを行わない
        if ( strncmp($text,'xmp',3)==0 ){
            $this->xmp--;
            //  最上位のxmpを閉じるときだけ成形する
            if ( $this->xmp<1 ){
                $text = substr($text,4);
                if ( preg_match('/^([ \t]+)/',$text,$match) ){
                    $indent = str_replace("\t",str_repeat(' ',4),$match[1]);
                    $text = str_replace(self::LF.$match[1],self::LF,$text);
                    $text = str_replace("\t",str_repeat(' ',4),$text);
                    $text = ltrim($text);
                }

                $text = str_replace('<','&lt;',$text);
                $text = str_replace('>','&gt;',$text);
                $text = str_replace(self::LF,'</br>',$text);
                $text = str_replace('  ','&nbsp;&nbsp;',$text);
                return str_replace(self::LF,'</br>'.self::LF,$text);
            }
        }

        if ( $this->xmp>0 ){
            return $this->tag_open .$open_lf. $text . $this->tag_close . $lf;
        }

        //  タグの直後に来ている１文字で処理を決める系
        if ( strlen($text)<1 ){
            $text='*';
        }
        $text = trim($text);
        switch($text[0]){
            case '*':   return '';
            case '&':   return '<?php echo htmlspecialchars('.substr($text,1).',ENT_QUOTES);?>'.$lf;
            case '%':   return '<?php echo rawurlencode('.substr($text,1).');?>'.$lf;
            case '#':   $var=substr($text,1); return '<?php echo isset('.$var.')?'.$var.':null;?>'.$lf;
            case '-':   
                $text = str_replace(';','&#x3b;',$text);
                //$temp = explode(' ',substr($text,1),2);
                $temp = preg_split('/\s+/',substr($text,1),2);
                if ( count($temp)==1 ){
                    $temp[1]='';
                }
                return '<?php $_=$__UTIL__->'.$temp[0].'(['.$temp[1].']); extract($_)?>'.$lf;
            //  空白やコントロールコード
            //  $text = trim($text);
            //  があるのでここのは来ない
            /**
            case ' ':
            case "\r":
            case "\n":
            case "\t":
                return $this->tag_open . $open_lf . $text . $lf . $this->tag_close . $lf;
            */
        }

        //  単語で動作を決める系
        //switch(trim($text)){
        switch($text){
            case '':        return $this->tag_open.$this->tag_close.$lf;
            case '/foreach':
            case '/if':
            case '/for':
            case '/while':
            case '/':      return '<?php } ?>'.$lf;
            case 'else':    return '<?php }else{ ?>'.$lf;
            case 'dump':
            case 'vdump':   return '<?php var_dump($this->assigns); ?>'.$lf;
            case 'quit':    return '<?php return TRUE; ?>'.$lf;
            case 'quitstop':return '<?php return FALSE; ?>'.$lf;
            case 'exit':    return '<?php exit(); ?>'.$lf;
            case 'break':   return '<?php break; ?>'.$lf;
            case 'continue':return '<?php continue; ?>'.$lf;
            case 'strip':   return '<?php ob_start(); ?>'.$lf;
            case '/strip':  return '<?php $_=ob_get_contents(); ob_end_clean(); echo self::strip($_); ?>'/*.$lf*/;
            case 'capture': $this->capture_vars[]='$_'; return '<?php ob_start(); ?>';
            case '/capture':
                $name = array_pop($this->capture_vars);
                if ( $name===null ){
                    trigger_error( "captureと/captureの呼出が対応していません。",E_USER_ERROR );
                }
                return "<?php {$name}=ob_get_contents(); ob_end_clean(); ?>";
        }

        //  変数・定数の類を表示
        if ( !preg_match(self::REGEXP,$text,$match) ){
            return '<?php echo '.$text.';?>'.$lf.$lf;
        }

        //  その他のコマンド
        $cmd  = $match[1];
        $text = substr($text,strlen($match[0])); //  構文直後の文字を空白１文字を抜いて取得

        switch($cmd){
            case 'if':
            case 'for': 
            case 'while':
                return '<?php '.$cmd.'('.$text.'){?>';
            case 'foreach':
                if ( preg_match('/@(\$\w+)\s*$/',$text,$match) ){
                    $text = substr($text,0,-strlen($match[0]));
                    return '<?php '.$match[1].'=-1; foreach('.$text.'){'.$match[1].'++;?>';
                }
                return '<?php $_=-1; foreach('.$text.'){$_++;?>';
            case '=':
            case 'code':
                return '<?php '.$text.'?>'.$lf;
            case 'namespace':
                $this->namespace=rtrim($text,';');
                return '';
            case 'import':
                //  子の終了コードが FALSE だったら、現在のテンプレート出力はその時点で終了する
                //  が、親のテンプレートは続行させたいのでTRUEを返す。
                return  '<?php list($__is_break__,$__time__,$sub)=$this->import(array('.$text.'));'.
                        'extract(array(\'__IMPORT_TIME__\'=>$__time__));'.
                        'if ($__is_break__){return TRUE; /*呼び出し元は止めない*/ }?>'.$lf;
            case 'xmp':
                //  ここには来ない
                break;

            case 'capture':
                $text = trim($text);
                if ( !preg_match('/^\$\w+$/',$text) ){
                    self::error( "capture で、変数として利用できない名前 \"{$text}\" が指定されました。" );
                }
                $this->capture_vars[]=$text;  //  変数名を保存
                return '<?php ob_start(); ?>';

            case 'elsif':
            default:    // = else if
                return '<?php } else if ('.$text.'){?>'.$lf;
            case 'html_radios':
                $params = explode(' ',$text);
                $selected = null;
                $addstr   = '';
                foreach( $params as $param ) {
                    $exp = explode('=',$param);
                    switch($exp[0]) {
                      case 'name'    : $name = $exp[1]; break;
                      case 'options' : $options = $exp[1]; break;
                      case 'selected': $selected = $exp[1]; break;
                      default : $addstr .= ' '.$param.' '; break;
                    }
                }
                $text  = "<?php foreach($options as \$k => \$v){?>";
                $text .= "<label><input type='radio' name='<?php echo \"$name\";?>' value='<?php echo \$k;?>'";
                if ( !is_null($selected) ) {
                    $text .= "<?php if($selected==\$k){?>checked='checked' <?php }?>";
                }
                $text .= " $addstr /><?php echo \$v;}?>\n";
                return $text;
          case 'html_options':
                $params = explode(' ',$text);
                $selected = null;
                $addstr   = '';
                foreach( $params as $param ) {
                    $exp = explode('=',$param);
                    switch($exp[0]) {
                      case 'name'    : $name = $exp[1]; break;
                      case 'options' : $options = $exp[1]; break;
                      case 'selected': $selected = $exp[1]; break;
                      default : $addstr .= ' '.$param.' '; break;
                    }
                }
                $text  = "<select name ='<?php echo \"$name\";?>' $addstr>";
                $text .= "<?php foreach($options as \$k => \$v){?>";
                $text .= "<option value='<?php echo \$k;?>'";
                if ( !is_null($selected) ) {
                    $text .= "<?php if($selected==\$k){?> selected <?php }?>";
                }
                $text .= "><?php echo \$v;?></option><?php }?>\n";
                return $text;
        }
    }

    //************************************************************************
    //  templateから呼び出すHTML用ヘルパー（別クラスがいいのかも）
    //************************************************************************
    /** 余分な空白を削除します。テンプレートファイル内から利用します。  */
    public static function strip($text)
    {
        //  時間計測したらcallbackの方が３倍くらい遅かった
        //  return trim(preg_replace_callback('/>([^<]+)</s',function($matches){return '>'.trim($matches[1]).'<';},$text));
        return preg_replace('/\s+</s',' <',preg_replace('/>\s+/s','> ',preg_replace('/>\s+</s','><',trim($text))));
    }

    /** 文字列中の改行コードを<br>に変換します。テンプレートファイル内から利用します。  */
    public static function br($value)
    {
        return str_replace("\n","<br>",$value);
    }

    /** 文字列中の空白を&nbsp;に変換します。テンプレートファイル内から利用します。  */
    public static function nbsp($value)
    {
        return str_replace(' ','&nbsp;',$value);
    }

    //************************************************************************
    //  テスト
    //************************************************************************
    /** PmlTemplateクラスの簡単な単体テストを行います。 */
    public static function test()
    {
        $template = new static();
        $result = $template->fetch(__DIR__.'/../../test/tpl/PmlTemplate/_frame.html',array('a'=>'b'),$force_compile=TRUE);
        echo($result);

        //  !found
        $template = new static('/tmp');
        $result = $template->fetch(__DIR__.'notFound',array('a'=>'b'));
        echo($result);

        //  
        $template = new static();
        $result = $template->fetch(__DIR__.'/test.txt',array('a'=>'b'));
        echo($result);
    }

}


