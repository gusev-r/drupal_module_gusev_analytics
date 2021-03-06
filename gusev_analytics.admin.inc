<?php

/**
 * @file
 * Administrative configuration form
 */

/**
 * Configuration form module.
 */
function gusev_analytics_users_page() {
    $return_array .= drupal_render(drupal_get_form('gusev_analytics_users_form'));
    $return_array .= _gusev_analytics_pages_info_render();

    if ($_SESSION['gusev_analytics_form_xml'] == TRUE) {
        $_SESSION['gusev_analytics_form_xml'] = FALSE;
        $filename = 'list.xls';
        header("Content-type: application/vnd.ms-excel; charset=UTF-8");
        header("Expires: 0");
        header("Cache-Control: must-revalidate, post-check=0,pre-check=0");
        header("Pragma: public");
        header('Content-Encoding: UTF-8');
        header('Content-type: text/csv; charset=UTF-8');
        header("Content-Disposition: attachment; filename=\"$filename\"");
        print "\xEF\xBB\xBF"; // UTF-8 BOM 
        print _gusev_analytics_pages_info_render();
        die;
    }
    
    return $return_array;
}

function gusev_analytics_users_form($form, &$form_state) {
    $filter_data = isset($_SESSION['gusev_analytics_users_form']) ? $_SESSION['gusev_analytics_users_form'] : array();

    $nowYear = date("Y");
    $iYear = date("Y") - 5;
    $year = array();
    for ($iYear; $iYear <= $nowYear; $iYear++) {
        $year[$iYear] = $iYear;
    }
    $form['year'] = array(
        '#type' => 'select',
        '#title' => t('Year'),
        '#options' => $year,
        '#default_value' => isset($filter_data['year'][0]) ? $filter_data['year'][0] : date("Y"),
    );

    $month = array(
        0 => t('All'),
        1 => 'Январь',
        2 => 'Февраль',
        3 => 'Март',
        4 => 'Апрель',
        5 => 'Май',
        6 => 'Июнь',
        7 => 'Июль',
        8 => 'Август',
        9 => 'Сентябрь',
        10 => 'Октябрь',
        11 => 'Ноябрь',
        12 => 'Декабрь'
    );
    $form['month'] = array(
        '#type' => 'select',
        '#title' => t('Month'),
        '#options' => $month,
        '#default_value' => isset($filter_data['month'][0]) ? $filter_data['month'][0] : date("m"),
    );
    $form['submit'] = array(
        '#value' => t('Save'),
        '#type' => 'submit',
        '#submit' => array('gusev_analytics_users_form_submit')
    );
    $form['submit_xls'] = array(
        '#value' => t('Generate XLS'),
        '#type' => 'submit',
        '#submit' => array('gusev_analytics_users_xls_submit')
    );
    return $form;
}

function gusev_analytics_users_form_submit($form, &$form_state, $state) {
    $_SESSION['gusev_analytics_users_form'] = array(
        'year' => array($form_state['values']['year']),
        'month' => array($form_state['values']['month']),
    );
}

function gusev_analytics_users_xls_submit($form, &$form_state, $state) {
    $_SESSION['gusev_analytics_form_xml'] = TRUE;
}