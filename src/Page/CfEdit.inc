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

use phpcassa\Schema\StrategyClass;
use phpcassa\Schema\DataType;
use cassandra\CfDef;


//========================================================================
/**
 *
 */
//========================================================================
class CfEdit extends PageBase
{
    private $params = array(
        'Main' => array(
            ),
        'Edit' => array(
            'options'  => NULL,
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
        $_cfdef         = $cas->ColumnFamily->cfdef;
        $columnMetadata = $cas->columnMetadata();

        $options        = array();
        $exclusion      = array('keyspace',
                                'name',
                                'column_metadata',
                                'id',
                                'compaction_strategy_options',
                                'compression_options'
                                );
        foreach( CfDef::$_TSPEC as $spec ) {
            if ( array_search($spec['var'],$exclusion) ===FALSE ) {
                $options[$spec['var']] = ucwords(str_replace('_',' ',$spec['var']));
            }
        }

        $useDataType = Constants::CfUseDataTypeOptions();
        $cfdef = array();
        foreach($_cfdef as $option => $value ) {
            if ( array_search($option,$useDataType) !== FALSE ) {
                $value = Cassandra::extractDataType($value);
            }
            $cfdef[$option] = $value;
        }
        
        $assigns += array(
            'cfdef'          => $cfdef,
            'options'        => $options,
            'columnMetadata' => $columnMetadata,
            'dataType'       => Constants::CfDataType(),
            'useDataType'    => $useDataType

            );
        foreach($this->params[__FUNCTION__] as $key => $value ) $assigns[$key] = $$key;
    }

    //========================================================================
    /**
     *
    */
    //========================================================================
    public function Edit(&$assigns) {
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
        $cas->Manager->alter_column_family(KEY_SPACE,COLUMN_FAMILY,$attrs);

        $url = '?view=CfStructure&ksName='.KEY_SPACE.'&cfName='.COLUMN_FAMILY;
        header("Location: $url");
        exit();
    }

}
?>
