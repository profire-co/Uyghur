<?php
/*
 * Uyghur
 * Copyright (c) 2014 Profire Co.,Ltd
 *
 * This software is released under the MIT License.
 *
 * http://opensource.org/licenses/mit-license.php
 */
namespace CassAdmin\Page;

use CassAdmin\Common;

//========================================================================
/**
 *
 */
//========================================================================
class PageBase
{
    protected $manager;
    protected $error;

    //========================================================================
    /**
     *
     */
    //========================================================================
    protected function __construct($manager,&$params)
      {
          $this->manager = $manager;
          if ( !empty($params[FUNC]) ) {
              foreach ( $params[FUNC] as $key => &$value ) {
                  $value = Common::getQueryString($key,$value);
              }
          }

      }
}
?>
