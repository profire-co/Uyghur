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

use phpcassa\ColumnFamily;
use phpcassa\ColumnSlice;
use phpcassa\SystemManager;
use phpcassa\Connection\ConnectionPool;
use phpcassa\Schema\DataType;

use cassandra\ColumnParent;
use cassandra\Compression;
use cassandra\CqlResultType;

//========================================================================
/**
 *
 */
//========================================================================
class Cassandra
{
    const DEFAULT_ROW_COUNT = 100;
    const CQL_SELECT        = 'select';
    const CQL_INSERT        = 'insert';
    const CQL_UPDATE        = 'update';
    const CQL_DELETE        = 'delete';
    
    public  $pool;
    public  $Manager;
    public  $ColumnFamily;

    
    //========================================================================
    /**
     *
    */
    //========================================================================
    public function __construct($manager,$ksName=NULL,$cfName=NULL)
    {
        $this->Manager      = $manager;
        if ( !is_null($ksName) ){
            $this->pool         = new ConnectionPool($ksName, array($manager->conn->server));
            if ( !is_null($cfName) ) {
                $this->ColumnFamily = new ColumnFamily($this->pool, $cfName);
            }
        }
    }

    //========================================================================
    /**
     *
    */
    //========================================================================
    public function columnMetadataName() {
        $cfDef         = $this->ColumnFamily->cfdef;
        $columnList    = array();
        foreach( $cfDef->column_metadata as $value) {
            array_push($columnList,$value->name);
        }
        return $columnList;
    }
    //========================================================================
    /**
     *
    */
    //========================================================================
    public function columnMetadata() {
        $cfDef         = $this->ColumnFamily->cfdef;
        $columnList    = array();
        foreach( $cfDef->column_metadata as $value) {
            array_push($columnList,array(
                'name'             => $value->name,
                'validation_class' => $value->validation_class,
                'index_type'       => $value->index_type,
                'index_name'       => $value->index_name,
                'index_options'    => $value->index_options,
                )
                       );
        }
        return $columnList;
    }
    //========================================================================
    /**
     *
    */
    //========================================================================
    public function cfDefs($keySpace) {
        $ksDef         = $this->Manager->describe_keyspace($keySpace);
        $cfDefs        = array();

        foreach( $ksDef->cf_defs as $cfDef ) {
            $set = array();
            foreach ( $cfDef as $column => $value ) {
                $set[$column] = $value;
            }
            $cfDefs[$set['name']] = $set;
        }
        ksort($cfDefs);
        return $cfDefs;
    }
    //========================================================================
    /**
     *
    */
    //========================================================================
    public function ksDefs() {
        $ksDefs        = $this->Manager->describe_keyspaces();
        $return        = array();

        foreach( $ksDefs as $ksDef ) {
            foreach ( $ksDef as $option => $value ) {
                $return[$ksDef->name][$option] = $value;
            }
            //array_push($return,$set);
        }
        ksort($return);
        return $return;
    }
    //========================================================================
    /**
     *
    */
    //========================================================================
    public function multiGet() {
        $arg    = func_get_args();
        return  call_user_func_array(array($this->ColumnFamily,'multiget'),$arg);
    }

    //========================================================================
    /**
     *
    */
    //========================================================================
    public function getRange() {
        $arg    = func_get_args();
        $result = call_user_func_array(array($this->ColumnFamily,'get_range'),$arg);
        $return = array();
        foreach( $result as $key => $value ) {
            //var_dump($value);
            $return[$key] = $value;
        }
        return $return;
    }
    //========================================================================
    /**
     *
    */
    //========================================================================
    public function CqlQuery($query) {
        //var_dump($query);
        $arg    = func_get_args();
        $type   = trim(substr(strtolower($query),0,strpos(strtolower($query),' ')));
        switch( $type ) {
          case self::CQL_SELECT:
            $result = call_user_func_array(array($this,'CqlSelect'),$arg);
            break;
          case self::CQL_INSERT:
          case self::CQL_UPDATE:
          case self::CQL_DELETE:
          default:
            $raw    = $this->pool->get();
            $result = $raw->client->execute_cql_query($query, Compression::NONE);
            break;
        }
        //var_dump($result);
        //exit();
        return $result;
    }

    //========================================================================
    /**
     *
    */
    //========================================================================
    public static function extractColumnFamilyFromCql($query) {
        return trim(substr($query,strpos(strtolower($query),'from')+4));
    }
    //========================================================================
    /**
     *
    */
    //========================================================================
    public function CqlSelect($query,$detail = true) {
        $columnFamily = self::extractColumnFamilyFromCql($query);
        if ( ($pos = strpos($columnFamily,' ')) !== FALSE ) {
            $columnFamily = substr($columnFamily,0,$pos);
        }

        if ( !isset($this->ColumnFamily) ) {
            $this->ColumnFamily = new ColumnFamily($this->pool, $columnFamily);
        }
        
        $raw    = $this->pool->get();
        $result = $raw->client->execute_cql_query($query, Compression::NONE);

        $col_type_dict = array();
        foreach( $result->schema->value_types as $column => $dataType) {
            $class = 'phpcassa\\Schema\\DataType\\'.$dataType;
            $col_type_dict[$column] = new $class;
        }
        $class = 'phpcassa\\Schema\\DataType\\'.$result->schema->default_value_type;
        $default_dict = new $class;

        $return = array();
        
        foreach( $result->rows as $row ) {
            $key   = $this->unpack_key($row->key);
            foreach( $row->columns as $column) {
                if ( isset($col_type_dict[$column->name]) ) {
                    $dtype = $col_type_dict[$column->name];
                }
                else {
                    $dtype = $default_dict;
                }
                if ( $detail ) {
                    $return[$key][$column->name] = array(
                        'name'      => $column->name,
                        'value'     => $dtype->unpack($column->value),
                        'timestamp' => $column->timestamp,
                        'ttl'       => $column->ttl,
                        );
                }
                else {
                    if ( $column->name ==  'KEY' ) continue; 
                    $return[$key][$column->name] = $dtype->unpack($column->value);
                }
            }
        }
        $result->rows = $return;
        return $result;
    }
    
    //========================================================================
    /**
     *
    */
    //========================================================================
    public static function qualify_class_name($data_type) {
        if ($data_type === null)
          return null;
        
        if (strpos($data_type, ".") === false)
          return "org.apache.cassandra.db.marshal.$data_type";
        else
          return $data_type;
    }
    //========================================================================
    /**
     *
    */
    //========================================================================
    public static function extractDataType($class_name) {
        return str_replace("org.apache.cassandra.db.marshal.",'',$class_name);
    }
    
    //========================================================================
    /**
     *
    */
    //========================================================================
    public function __call($name, $arguments)
    {
        if ( method_exists($this->ColumnFamily,$name) ) {
            return call_user_func_array(array($this->ColumnFamily,$name),$arguments);
        }
        else {
            throw new \Exception(__CLASS__.' Undefined Method '.$name);
        }
    }
    //========================================================================
    /**
     *
    */
    //========================================================================
    public function getDetail($rowKey,$column_family) {
        
        $ColumnFamily = new ColumnFamily($this->pool, $column_family);

        $dataType  = $this->extractDataType($ColumnFamily->cfdef->key_validation_class);
        $key_alias = Common::strict($ColumnFamily->cfdef->key_alias,'KEY');
        
        switch( $dataType ) {
          case DataType::BYTES_TYPE:
            $Key = '';
            for($i = 0; $i< strlen($rowKey);$i++ ) {
                $Key .= sprintf("%x",ord($rowKey[$i]));
            }
            break;
          case DataType::UTF8_TYPE:
            $Key = "'{$rowKey}'";
            break;
          default:
            $Key = $rowKey ;
            break;
        }
        
        $query = 'SELECT * FROM '.$column_family.' WHERE '.$key_alias.'='. $Key;
        $result = $this->CqlQuery($query,true);

        return $result->rows[$rowKey];
    }

}
?>
