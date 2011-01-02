<?php

class BsdApi {
  
  var $api_id;
  var $api_secret;
  
  var $http_request_base;
  var $http_request_options;
  
  var $deferred_result_call_interval;
  var $deferred_result_call_max_attempts;
  
  var $output_format;
  
  const HTTP_CODE_OK = 200;
  const HTTP_CODE_DEFERRED_RESULT = 202;
  const HTTP_CODE_DEFERRED_RESULT_COMPILING = 503;
  
  const OUTPUT_40CHARHEX = 1;
  const OUTPUT_BASE64 = 2;
  
  const API_VER = 1;

  public function __construct(){
    // store vars requied by BSD API
    $this->http_request_base = variable_get('bsdapi_url', ''); //'http://socialcontxt.bsdtoolsdemo.com/page/api/';
    $this->api_id = variable_get('bsdapi_key', ''); 
    $this->api_secret = variable_get('bsdapi_secret', '');  
    
    // set deferred result defaults
    $this->deferred_result_call_interval = variable_get('deferred_result_call_interval', '5'); // in seconds
    $this->deferred_result_call_max_attempts = variable_get('deferred_result_call_max_attempts', '20'); 
    
    
    $this->output_format = self::OUTPUT_40CHARHEX;
    
    $this->http_request_options = array(
        'timeout' => 10, 
        'readTimeout' => array(10, 0), 
        'allowRedirects' => true, 
        'maxRedirects' => 3 
    );
  }

  public function request($url, $query_params = array()){
    // prepend URL with base path for the API
    $url = $this->http_request_base . $url;
    
    // add api_id, timestamp, and version number to query string
    $query_params['api_id'] = $this->api_id;
    if (! array_key_exists('api_ts', $query_params)) {
      $query_params['api_ts'] = time();
    }
    $query_params['api_ver'] = self::API_VER;
    
    // add api_mac to query string after using existing query and request url to build
    // the api_mac
    $query_params['api_mac'] = $this->_buildApiMac($url, $query_params);
    
    $url .= '?';
    $url .= $this->_attrs_to_query($query_params);
    
    $result = drupal_http_request($url, $this->http_request_options);
    
    // is this a deferred result?
    if ($result->code == self::HTTP_CODE_DEFERRED_RESULT) {
      $result = $this->_deferredResult($result->data);
    }
    //dpm($result);
    return $result->data;
  }

  /**
   * Calculate hash
   * 
   * @param $url
   * @param $query_params
   * @return unknown_type
   */
  private function _buildApiMac($url, $query_params){
    // break URL into parts to get the path
    // i.e. "/page/api/cons/get_constituents_by_id"
    $url_parts = parse_url($url);

    // build query string from given parameters
    // $query_string = urldecode(http_build_query($query_params));  // does NOT work
    // Instead ..    
    $query_string = $this -> _attrs_to_query($query_params);

    // combine strings to build the signing string
    $signing_string = $query_params['api_id'] . "\n" . $query_params['api_ts'] . "\n" . $url_parts['path'] . "\n" . $query_string;
    
    // calculate hash
    $hash = hash_hmac('sha1', $signing_string, $this->api_secret);
    
    if ($this->output_format == self::OUTPUT_BASE64) {
      $hash = $this->_hex2b64($hash);
    }
    // else, output_format is the default self::OUTPUT_40CHARHEX
    return $hash;
  
  }

  private function _deferredResult($deferred_id){
    $attempt = 0;
    
    // loop until result is ready or until we give up
    do {
      // delay between calls (in seconds)
      sleep($this->deferred_result_call_interval);
      
      // check to see if result is ready
      $req = $this->request('get_deferred_results', array(
          'deferred_id' => $deferred_id 
      ));
      
      // increment attempts counter
      $attempt ++;
    } while ( $req->code == self::HTTP_CODE_DEFERRED_RESULT_COMPILING && $attempt < $this->deferred_result_call_max_attempts );
    
    // if the response code isn't HTTP_CODE_OK then we didn't get the result we wanted
    if ($req->code != self::HTTP_CODE_OK) {
      // did we go over our "max attempts"?
      if ($iteration >= $this->deferred_result_call_max_attempts) {
        throw new Exception('Could not retrieve deferred result.  Max attempts reached.', 1);
      }
      // we must have received an unexpected HTTP code
      else {
        throw new Exception('Could not retrieve deferred result.  HTTP Code ' . $req->code . '  was returned, with the following message: ' . $req->data, 2);
      }
    }
    
    // return request result
    return $req;
  }

  protected function _hex2b64($str){
    $raw = '';
    for($i = 0; $i < strlen($str); $i += 2) {
      $raw .= chr(hexdec(substr($str, $i, 2)));
    }
    return base64_encode($raw);
  }
  
  protected function _attrs_to_query($attrs){
    $d = array();
    foreach ( $attrs as $key => $value ) {
      $d[] = $key . '=' . $value;
    }
    
    return implode('&', $d);
  }

}