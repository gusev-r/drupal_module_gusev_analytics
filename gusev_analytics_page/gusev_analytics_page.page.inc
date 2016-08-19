<?php

/**
 * @file
 * Administrative configuration form
 */

/**
 * Configuration form module.
 */
function gusev_analytics_admin_page() {
    $return_array .= drupal_render(drupal_get_form('gusev_analytics_pages_form'));
    $return_array .= _gusev_analytics_page_statistic_render();
    return $return_array;
}

function gusev_analytics_pages_form($form, &$form_state) {
    $filter_data = isset($_SESSION['gusev_analytics_pages_form']) ? $_SESSION['gusev_analytics_pages_form'] : array();
    $type = array(
        0 => 'Просмотры',
        1 => 'Уникальные просмотры'
    );
    $form['type'] = array(
        '#type' => 'select',
        '#title' => t('Type'),
        '#options' => $type,
        '#default_value' => isset($filter_data['type'][0]) ? $filter_data['type'][0] : 0,
    );
    $form['submit'] = array(
        '#value' => t('Save'),
        '#type' => 'submit',
        '#submit' => array('gusev_analytics_pages_form_submit')
    );
    return $form;
}

function gusev_analytics_pages_form_submit($form, &$form_state, $state) {
    $_SESSION['gusev_analytics_pages_form'] = array(
        'type' => array($form_state['values']['type']),
    );
}
