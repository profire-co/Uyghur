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

//========================================================================
/**
 *
 */
//========================================================================
class Common
{

    //========================================================================
    /**
     *
     */
    //========================================================================
    public static function strict(&$value,$default=NULL)
      {
          return isset($value)?$value:$default;
      }

    //========================================================================
    /**
     *
     */
    //========================================================================
    public static function getQueryString($name,$def=NULL)
      {
          if      ( isset($_GET[$name]) && $_GET[$name]!='' ){
              return $_GET[$name];
          }
          else if ( isset($_POST[$name]) && $_POST[$name]!='' ){
              return $_POST[$name];
          }
          else if ( isset($_COOKIE[$name]) && $_COOKIE[$name]!='' ){
              return $_COOKIE[$name];
          }
          return $def;
      }

}

?>
