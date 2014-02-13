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
use CassAdmin\Constants;
use CassAdmin\Cassandra;

//========================================================================
/**
 *
 */
//========================================================================
class CfSearch extends PageBase
{
    private $params = array(
        'Main'   => array(
            ),
        );
    public  $error;
    private $session;
    
    //========================================================================
    /**
     *
    */
    //========================================================================
    public function __construct($manager)
    {
        parent::__construct($manager,$this->params);
        $this->session = &$_SESSION[__CLASS__];
                
        if ( is_null(COLUMN_FAMILY) ) {
            $this->error = true;
        }
    }

    //========================================================================
    /**
     *
    */
    //========================================================================
    public function Main(&$assigns) {
        extract($this->params[__FUNCTION__]);

        foreach($this->params[__FUNCTION__] as $key => $value ) $assigns[$key] = $$key;
    }

}
?>
