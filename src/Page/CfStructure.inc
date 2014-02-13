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
class CfStructure extends PageBase
{
    private $params = array(
        'Main' => array(
            ),
        'Create' => array(
            'options'  => NULL,
            'back'     => NULL,
            ),
        'Drop' => array(
            'back'     => NULL,
            ),
        'Truncate' => array(
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
    public function Main(&$assigns) {
        extract($this->params[__FUNCTION__]);

        $cas            = new Cassandra($this->manager,KEY_SPACE,COLUMN_FAMILY);
        $cfdef          = $cas->ColumnFamily->cfdef;
        $columnMetadata = $cas->columnMetadata();
        
        
        $assigns += array(
            'cfdef'          => $cfdef,
            'columnMetadata' => $columnMetadata,
            'dataType'       => Constants::CfDataType(),
            );
        foreach($this->params[__FUNCTION__] as $key => $value ) $assigns[$key] = $$key;
    }

    //========================================================================
    /**
     *
    */
    //========================================================================
    public function Create(&$assigns) {
        extract($this->params[__FUNCTION__]);

        $cas            = new Cassandra($this->manager,KEY_SPACE);
        $attrs          = array();
        
        foreach( $options as $key => $value ) {
            if ( $value != '' ) {
                if ( array_search($key,Constants::CfUseDataTypeOptions()) !== FALSE ) {
                    $value = Cassandra::qualify_class_name($value);
                }
                $attrs[$key] = $value;
            }
        }
        
        $attrs = empty($attrs)?NULL:$attrs;
        $cas->Manager->create_column_family(KEY_SPACE,COLUMN_FAMILY,$attrs);
        
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
        $cas->Manager->drop_column_family(KEY_SPACE,COLUMN_FAMILY);
        
        if ( is_null($back) ) {
            $back = '?view=KsStructure&ksName='.KEY_SPACE;
        }
        else {
            $back = urldecode($back);
        }

        header("Location: $back");
        exit();
    }
    //========================================================================
    /**
     *
    */
    //========================================================================
    public function Truncate(&$assigns) {
        extract($this->params[__FUNCTION__]);

        $cas            = new Cassandra($this->manager,KEY_SPACE,COLUMN_FAMILY);
        $cas->Manager->truncate_column_family(KEY_SPACE,COLUMN_FAMILY);
        
        if ( is_null($back) ) {
            $back = '?view=KsStructure&ksName='.KEY_SPACE;
        }
        else {
            $back = urldecode($back);
        }

        header("Location: $back");
        exit();
    }

}
?>
