<?php
/*
 * Uyghur
 * Copyright (c) 2014 Profire Co.,Ltd
 *
 * This software is released under the MIT License.
 *
 * http://opensource.org/licenses/mit-license.php
 */
namespace CassAdmin;

use pml\alone\Poult;
use CassAdmin\Page;

use phpcassa\ColumnFamily;
use phpcassa\ColumnSlice;
use phpcassa\SystemManager;

//========================================================================
/**
 *
 */
//========================================================================
class CassAdmin
{
    private $manager;
    private $poult;
    private $params = array(
        'Main' => array(
            'ksName' => NULL,
            'cfName' => NULL,
            'view'   => 'KsStructure',
            'func'   => 'Main',
            'skin'   => NULL,
            ),
        );
    private $e = NULL;

    //========================================================================
    /**
     *
     */
    //========================================================================
    public function __construct($host=NULL,$port=NULL)
      {
          if ( is_null($host) || is_null($port) ) {

          }
          else {
              try {
                  $this->manager = new SystemManager($host.':'.$port);
                  define( 'CLUSTER_NAME' , $this->manager->describe_cluster_name());
              }
              catch(\Exception $e) {
                  $this->e = $e;
              }
          }

          $this->poult = new Poult(COMPILE_DIR);
          foreach ( $this->params['Main'] as $key => &$value ) {
              $value = Common::getQueryString($key,$value);
          }
      }

    //========================================================================
    /**
     *
     */
    //========================================================================
    public function Main($ini) {
        extract($this->params[__FUNCTION__]);

        $assigns   = array();

        if (  $this->manager ) {
            define('COLUMN_FAMILY',$cfName);
            define('FUNC'         ,$func);

            $keySpaces = $this->manager->describe_keyspaces();
            $ksOptions = array();
            foreach( $keySpaces as $keySpace )
              $ksOptions[$keySpace->name] = $keySpace->name.' ('.count($keySpace->cf_defs).')';

            ksort($ksOptions);
            $ksName = Common::strict($ksName,key($ksOptions));
            foreach( $keySpaces as $keySpace ) {
                if ( $ksName == $keySpace->name ) {
                    $ksDef = $keySpace;
                }
            }
            define('KEY_SPACE'    ,$ksName);

            $cfList = array();
            if ( isset($ksDef) ) {
                foreach($ksDef->cf_defs as $cfDef ) {
                    array_push($cfList,$cfDef->name);
                }
                sort($cfList);
            }

            if ( $view != 'Main' ) {
                $class = $view;
                if ( class_exists (__NAMESPACE__.'\\Page\\'.$class) ) {
                    $class = __NAMESPACE__.'\\Page\\'.$class;
                    $class = new $class($this->manager);
                    if ( $class->error == true ) {

                    }
                    else if ( method_exists($class,$func)) {
                        try {
                            call_user_func_array(array($class,$func),array(&$assigns));
                            if ( isset($class->template_file) ) {
                                $template_file = $class->template_file;
                            }
                        }
                        catch(\Exception $e) {
                            $message = $e->getMessage();

                            $assigns += array(
                                'message' => Common::strict($message),
                                'class'   => get_class($e),
                                'code'    => $e->getCode(),
                                'file'    => $e->getFile(),
                                'line'    => $e->getLine(),
                                'trace'   => $e->getTraceAsString(),
                                );

                            $template_file = 'Error';
                        }
                    }

                }

            }
        }

        if      ( isset($template_file) &&
                  file_exists(TEMPLATE_DIR.$template_file.'.tpl') ) {
        }
        else if ( file_exists(TEMPLATE_DIR.$view.'/'.$func.'.tpl') ) {
            $template_file = $view.'/'.$func;
        }
        else if ( file_exists(TEMPLATE_DIR.$view.'.tpl') ) {
            $template_file = $view;
        }
        else {
            $template_file = 'Main';
        }

        define('VIEW' ,$view);

        $servers = array();
        foreach( $ini as $key=> $value ) if ( !empty($key) ) $servers[$key] = "$key({$value['host']})";

        $skins   = array('default');
        if ($handle = opendir(CSS_DIR)) {
            while (false !== ($css_file = readdir($handle))) {
                if ( preg_match('/^(skin\.)(.*)(\.css)/',$css_file,$matches) ) {
                    if ( $matches[2] != 'default' ) {
                        array_push($skins,$matches[2]);
                    }
                }
            }
        }

        if ( defined('CLUSTER_NAME') ) {
            $cookie_key = urlencode(str_replace(' ','',CLUSTER_NAME).'_SKIN');
            if ( !is_null($skin)  ) {
                setcookie($cookie_key,$skin,0,dirname($_SERVER['REQUEST_URI']));
            }
            else {
                $list = array_keys($servers);
                $no   = array_search(CLUSTER_NAME,$list);
                $no   = (count($skins)>$no)?$no:$no%count($skins);
                $skin = Common::getQueryString($cookie_key,$no);
            }
        }

        $eMessage = ($this->e != null)?$this->e->getMessage():NULL;
        
        $assigns += array(
            'MAIN_TEMPLATE'    => $template_file,
            'MENU_servers'     => $servers,
            'MENU_ksDef'       => Common::strict($ksDef,NULL),
            'MENU_ksOptions'   => Common::strict($ksOptions),
            'MENU_cfList'      => Common::strict($cfList),
            'MENU_skin'        => Common::strict($skin,0),
            'MENU_skins'       => $skins,
            'ERROR'            => Common::strict($eMessage),
            );

        $display = $this->poult->fetch(TEMPLATE_DIR.'index.tpl',$assigns);
        echo($display);
    }
}
?>
