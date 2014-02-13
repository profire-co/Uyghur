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

use phpcassa\Schema\StrategyClass;
use phpcassa\Schema\DataType;
use cassandra\CfDef;

//========================================================================
/**
 *
 */
//========================================================================
class KsStructure extends PageBase
{
    private $params = array(
        'Main' => array(
            ),
        'Create' => array(
            'name' => '',
            'strategy_class'     => '',
            'replication_factor' => 1,
            'back' => '',
            
            ),
        'Drop'  => array(
            'ksName' => NULL,
            'back'   => NULL,
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

    }

    //========================================================================
    /**
     *
    */
    //========================================================================
    public function Main(&$assigns) {
        extract($this->params[__FUNCTION__]);

        $cas            = new Cassandra($this->manager,KEY_SPACE);
        $ksDef          = $cas->Manager->describe_keyspace(KEY_SPACE);
        $cfDefs         = $cas->cfDefs(KEY_SPACE);

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

        $dataType = array('' => 'Default' );
        $dataType += Constants::CfDataType();
            
        $assigns += array(
            'ksDef'       => $ksDef,
            'cfDefs'      => $cfDefs,
            'options'     => $options,
            'dataType'    => $dataType,
            'useDataType' => Constants::CfUseDataTypeOptions()
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

        $cas            = new Cassandra($this->manager,KEY_SPACE,COLUMN_FAMILY);
        if ( !empty($name)  ) {
            $attrs = array(
                'strategy_class'   => $strategy_class,
                
                );
            if ( $strategy_class != StrategyClass::NETWORK_TOPOLOGY_STRATEGY ) {
                $attrs['strategy_options'] = array(
                    'replication_factor' => $replication_factor
                    );
            }
            /*
            else {
                $attrs['strategy_options'] = array(
                    'datacenter1' => 1,
                    );
            }
             */
            $cas->Manager->create_keyspace($name,$attrs);
        }

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
        if ( !empty($ksName)  ) {
            $cas->Manager->drop_keyspace($ksName);
        }

        if ( is_null($back) ) {
            $back = '?clusterName='.CLUSTER_NAME.'&view=Cluster';
        }
        else {
            $back = urldecode($back);
        }
        
        header("Location: $back");
    }
}
?>
