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

    if ($_SESSION['gusev_analytics_pages_form_xml'] == TRUE) {
        $_SESSION['gusev_analytics_pages_form_xml'] = FALSE;
        $filename = 'list.xls';
        header("Content-type: application/vnd.ms-excel; charset=UTF-8");
        header("Expires: 0");
        header("Cache-Control: must-revalidate, post-check=0,pre-check=0");
        header("Pragma: public");
        header('Content-Encoding: UTF-8');
        header('Content-type: text/csv; charset=UTF-8');
        header("Content-Disposition: attachment; filename=\"$filename\"");
        print "\xEF\xBB\xBF"; // UTF-8 BOM 
        print _gusev_analytics_page_statistic_render();
        die;
    }

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

    // <special code>
    $specialization = $terms = entity_load('taxonomy_term', FALSE, array('vid' => 10));
    if ($specialization) {
        $options_specialization['-1'] = t('All');
        foreach ($specialization as $term) {
            $options_specialization[$term->tid] = str_repeat('-', $term->depth) . $term->name;
        }
        $form['specialization'] = array(
            '#type' => 'select',
            '#title' => t('Type'),
            '#options' => $options_specialization,
            '#default_value' => isset($filter_data['specialization'][0]) ? $filter_data['specialization'][0] : key($options_specialization),
        );
    }
    // </special code>
    $form['submit'] = array(
        '#value' => t('Save'),
        '#type' => 'submit',
        '#submit' => array('gusev_analytics_pages_form_submit')
    );
    $form['submit_xls'] = array(
        '#value' => t('Generate XLS'),
        '#type' => 'submit',
        '#submit' => array('gusev_analytics_pages_form_xls_submit')
    );
    return $form;
}

function gusev_analytics_pages_form_submit($form, &$form_state, $state) {
    $_SESSION['gusev_analytics_pages_form'] = array(
        'type' => array($form_state['values']['type']),
        'specialization' => array($form_state['values']['specialization']),
    );
}
function gusev_analytics_pages_form_xls_submit($form, &$form_state, $state) {
    $_SESSION['gusev_analytics_pages_form_xml'] = TRUE;
}