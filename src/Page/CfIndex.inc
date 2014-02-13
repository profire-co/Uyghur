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

use phpcassa\SystemManager;

//========================================================================
/**
 *
 */
//========================================================================
class CfIndex extends PageBase
{
    private $params = array(
        'Create' => array(
            'column'     => NULL,
            'dataType'   => NULL,
            'name'       => NULL,
            'back'       => NULL,
            ),
        'Drop' => array(
            'column'   => NULL,
            'back'     => NULL,
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
    public function Create(&$assigns) {
        extract($this->params[__FUNCTION__]);

        $cas       = new Cassandra($this->manager,KEY_SPACE,COLUMN_FAMILY);
        $dataType  = Cassandra::qualify_class_name($dataType);
        $results = $cas->Manager->create_index(KEY_SPACE,COLUMN_FAMILY,$column,$dataType,$name);

        $back = urldecode($back);
        header("Location: $back");
        exit();
    }

    //========================================================================
    /**
     *
    */
    //========================================================================
    public function Drop(&$assigns) {
        extract($this->params[__FUNCTION__]);

        $cas            = new Cassandra($this->manager,KEY_SPACE,COLUMN_FAMILY);
        $cas->Manager->drop_index(KEY_SPACE,COLUMN_FAMILY,$column);
        
        $back = urldecode($back);
        header("Location: $back");
        exit();
    }

}
?>
