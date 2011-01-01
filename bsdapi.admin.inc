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
  
  return system_settings_form($form);
}