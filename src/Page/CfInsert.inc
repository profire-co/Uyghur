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
class CfInsert extends PageBase
{
    private $params = array(
        'Main' => array(
            'rowKeys'   => array(),
            'back'      => NULL,
            ),
        'Update' => array(
            'rowKeys'   => array(),
            'columns'   => array(),
            'values'    => array(),
            'nulls'     => array(),
            'back'      => NULL,
            )
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

        $cas        = new Cassandra($this->manager,KEY_SPACE,COLUMN_FAMILY);
        $columnList = $cas->columnMetadataName();

        $rows    = array();
        if ( !empty ($rowKeys) ) {
            $rows = $cas->multiGet($rowKeys);
        }
        else {
            $rowKeys = array('');
        }
        
        
        $list = array();
        foreach( $rows as $rowKey => $row ) {
            foreach( $columnList as $column ) {
                $list[$rowKey][$column] = Common::strict($row[$column]);
            }
        }
        foreach( $rows as $rowKey => $row ) {
            foreach( $row as $column => $value ) {
                if ( !isset($list[$rowKey][$column]) ) {
                    $list[$rowKey][$column] = $value;
                }
            }
        }

        $columnStr = '';
        if ( !empty($columnList) ) {
            $columnStr  = "'";
            $columnStr .=implode("','",$columnList);
            $columnStr .= "'";
        }

        $this->session['list'] = $list;

        $assigns += array(
            'list'       => $list,
            'columnList' => $columnList,
            'columnStr'  => $columnStr,
        );
        foreach($this->params[__FUNCTION__] as $key => $value ) $assigns[$key] = $$key;
    }
    //========================================================================
    /**
     *
    */
    //========================================================================
    public function Update(&$assigns) {
        extract($this->params[__FUNCTION__]);
        $cas        = new Cassandra($this->manager,KEY_SPACE,COLUMN_FAMILY);
        
        foreach( $rowKeys as $index => $rowKey ) {
            if ( $rowKey=='' ) continue;
            
            $columnList    = $columns[$index];
            $valueList     = $values[$index];
            $nulls[$index] = !isset($nulls[$index])?array():array_keys($nulls[$index]);
            
            $setColumns    = array();
            foreach( $columnList as $cIndex => $column ) {
                if ( strpos($valueList[$cIndex],'1.0E+') === 0 ) continue;
                if ( empty($nulls[$column]) ) {
                    $setColumns[$column] = $valueList[$cIndex];
                }
            }
            $cas->insert($rowKey,$setColumns);
        }
        
        foreach( $rowKeys as $index=> $rowKey ) {
            $columnList  = $columns[$index];
            $nullList    = $nulls[$index];
            if ( !isset($this->session['list'][$rowKey])) {
                continue;
            }

            foreach ($this->session['list'][$rowKey] as $column => $value ) {
                if ( is_null($value) ) continue;
                if ( array_search($column,$columnList) !== FALSE ) continue;
                if ( array_search($column,$nullList) === FALSE ) {
                    array_push($nullList,$column);
                }
            }
            if ( !empty($nullList) ) {
                $cas->remove($rowKey,$nullList);
            }
        }
        
        if ( is_null($back) ) {
            $back = '?view=CfBrowse&func=View&ksName='.KEY_SPACE.'&cfName='.COLUMN_FAMILY;
        }
        header("Location: $back");
        exit();
    }
}
?>
