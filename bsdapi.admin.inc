<?php


/**
 * Settings form for global fb_social custimazation 
 */
function bsdapi_settings_form() {
  $form['bsdapi'] = array(
    '#collapsible'   => TRUE,
    '#title'         => t('Blue State Digital API information'),
    '#type'          => 'fieldset',
	
  );
  
  $form['bsdapi']['bsdapi_url'] = array(
    '#default_value' => variable_get('bsdapi_url', ''),
    '#title'         => t('The base URL of the API'),
    '#type'          => 'textfield',
	'#required'  => TRUE,
  );
  
  $form['bsdapi']['bsdapi_key'] = array(
    '#default_value' => variable_get('bsdapi_key', ''),
    '#title'         => t('Blue State Digital API key'),
    '#type'          => 'textfield',
	'#required'  => TRUE,
  );

   $form['bsdapi']['bsdapi_secret'] = array(
    '#default_value' => variable_get('bsdapi_secret', ''),
    '#title'         => t('Blue State Digital Shared Secret key'),
    '#type'          => 'textfield',
	'#required'  => TRUE,
  );
  
   $form['bsdapi']['deferred_result_call_interval'] = array(
    '#default_value' => variable_get('deferred_result_call_interval', '30'),
    '#title'         => t('Deferred call interval in seconds'),
    '#type'          => 'textfield',
  );
  
   $form['bsdapi']['deferred_result_call_max_attempts'] = array(
    '#default_value' => variable_get('deferred_result_call_max_attempts', '20'),
    '#title'         => t('Max attempts for the deferred result calls'),
    '#type'          => 'textfield',
  );
  
  return system_settings_form($form);
}