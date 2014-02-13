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

use phpcassa\Schema\DataType;

//========================================================================
/**
 *
 */
//========================================================================
class CfBrowse extends PageBase
{
    private $params = array(
        'View' => array(
            'mode'      => NULL,
            'dateTime'  => 'raw',
            'offsetKey' => '',
            'pageLimit' => 30,
            'page'      => 1,
            'next'      => NULL,
            'prev'      => NULL,
            'rowKeys'   => NULL,
            ),
        'Delete' => array(
            'rowKeys'   => array(),
            'back'      => NULL,
            ),
        'Detail' => array(
            'rowKey'    => NULL,
            'back'      => NULL,
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
    public function View(&$assigns){
        extract($this->params[__FUNCTION__]);
        $session = &$this->session[__FUNCTION__][KEY_SPACE][COLUMN_FAMILY];

        $cas                = new Cassandra($this->manager,KEY_SPACE,COLUMN_FAMILY);
        $columnMetadataName = $cas->columnMetadataName();
        $columnMetadata     = $cas->columnMetadata();
        
        if ( is_null($mode) ) {
            if ( empty($columnMetadataName) ) {
                $mode = 'kvs';
            }
            else {
                $mode = 'rdbms';
            }
        }

        if ( $page > 1 ) {
            if ( isset($session[$pageLimit][$page-1]) ) {
                $offsetKey = $session[$pageLimit][$page-1];
            }
            else {
                $page = 1;
            }
        }
        
        $queryLimit = ($page == 1 )?$pageLimit:$pageLimit+1;
        
        if ( is_null($rowKeys) ) {
            $casResult  = $cas->getRange($offsetKey,'',$queryLimit);
        }
        else {
            foreach( $rowKeys as $index=> $rowKey ) {
                if ($rowKey == '' ) {
                    unset($rowKeys[$index]);
                }
            }
            $casResult  = $cas->multiget($rowKeys);
        }
        
        $keyList    = array();
        $keyEmpty   = true;
        $columnList = array();
        $rdbmsList  = array();
        $kvsList    = array();
        $kvsEmpty   = true;
        $kvResult   = array();
        $dateColumn = array();

        if ( $dateTime == 'date') {
            foreach($columnMetadata as $column ) {
                $dataType = $cas->extractDataType($column['validation_class']);
                if ( $dataType == DataType::DATE_TYPE ) {
                    array_push($dateColumn,$column['name']);
                }
            }
            
        }

        foreach($casResult as $key=> $value ) {
            foreach( $value as $column_key => &$column_value ) {
                if (array_search($column_key,$dateColumn) !== FALSE)  {
                    $second = substr($column_value,0,10);
                    $micro  = substr($column_value,11);
                    $column_value   = date("Y-m-d H:i:s",$second);
                    $column_value  .= '.'.$micro;
                }
            }
            
            array_push($keyList,$key);
            $keyEmpty        = false;
            $kvResult[$key]  = $value;
            $rdbmsList[$key] = array();
            $kvsList[$key]   = array();
        }
        
        if ( count($kvResult) == $queryLimit ) {
            $session[$pageLimit][$page] = $key;
        }

        if ( $page > 1 ) {
            reset($kvResult);
            reset($keyList);
            $key = key($kvResult);
            if ( $key != $offsetKey ) {
                unset($session[$pageLimit][$page-1]);
                $this->params[__FUNCTION__]['page'] = 1;
                return $this->View($assigns);
            }
            unset($kvResult[$key]);
            $key = key($keyList);
            unset($keyList[$key]);
        }
        
        
        switch($mode) {
          case 'kvs':
            break;
          case 'rdbms':
            $columnList = $columnMetadataName;
            break;
          case 'rdbmsF':
            foreach( $kvResult as $key=> $values ) {
                foreach( $values as $column => $value ) {
                    if ( array_search($column,$columnList) === FALSE ) {
                        array_push($columnList,$column);
                    }
                }
            }
            sort($columnList);
            break;
        }
        
        foreach( $kvResult as $key => $values ) {
            foreach( $values as $column => $value ) {
                if ( array_search($column,$columnList) !== FALSE ) {
                    $rdbmsList[$key][$column] = $value;
                }
                else {
                    $kvsList[$key][$column] = $value;
                    $kvsEmpty      = false;
                }
            }
        }

        $beginCount = (($page-1)*$pageLimit)+1;
        $endCount   = (count($kvsList)==$pageLimit)?$beginCount+$pageLimit-1:$beginCount+count($kvsList)-1;
        
        $assigns += array(
            'keyEmpty'       => $keyEmpty,
            'keyList'        => $keyList,
            'kvsEmpty'       => $kvsEmpty,
            'kvsList'        => $kvsList,
            'rdbmsList'      => $rdbmsList,
            'columnList'     => $columnList,
            'modeCheckboxes' => Constants::CfBrowseViewMode(),
            'nextKey'        => Common::strict($session[$pageLimit][$page],''),
            'prevKey'        => Common::strict($session[$pageLimit][$page-2],''),
            'beginCount'     => $beginCount,
            'endCount'       => $endCount,
            'check'          => Common::getQueryString('check'),
            'dateTimeRadio'  => Constants::CfBrowseViewDateType(),
            'dateColumn'     => $dateColumn,
            );
        
        foreach($this->params[__FUNCTION__] as $key => $value ) $assigns[$key] = $$key;
    }
    //========================================================================
    /**
     *
    */
    //========================================================================
    public function Delete(&$assigns) {
        extract($this->params[__FUNCTION__]);

        $cas        = new Cassandra($this->manager,KEY_SPACE,COLUMN_FAMILY);

        foreach( $rowKeys as $rowKey ) {
            $rowKey;
            $cas->remove($rowKey);
        }
        header("Location: $back");
        exit();
    }
        //========================================================================
    /**
     *
    */
    //========================================================================
    public function Detail(&$assigns) {
        extract($this->params[__FUNCTION__]);

        $cas     = new Cassandra($this->manager,KEY_SPACE,COLUMN_FAMILY);
        $result  = $cas->getDetail($rowKey,COLUMN_FAMILY);

        foreach( $result as &$column ) {
            if ( $column['timestamp'] > 0 ) {
                $second = substr($column['timestamp'],0,10);
                $micro  = substr($column['timestamp'],10);

                $date   = date("Y-m-d H:i:s",$second);
                $date  .= '.'.$micro;
            }
            $column['timestamp_date'] = Common::strict($date);
        }
        
        
        $assigns += array(
            'result' => $result,
            );
        foreach($this->params[__FUNCTION__] as $key => $value ) $assigns[$key] = $$key;
    }

}
?>
