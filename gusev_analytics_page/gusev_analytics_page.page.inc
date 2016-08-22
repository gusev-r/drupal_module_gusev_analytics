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
        
        
        $options_specialization = array(
            '-1' => t(All),
            '9712' => 'Кардиология',
            '9658' =>'Терапия',
            '305016' => 'Педиатрия',
            '11188' =>'Гастроэнтерология',
            '9710' =>'Пульмонология и Аллергология',
            '10779' => 'Акушерство и гинекология',
            '9665' => 'Неврология',
            '9728' => 'Эндокринология',
            '305025'=>'Другое',
            '-200' => '--------------------',
        );
        
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

    $format = 'Y-m-d';

    $form['date_begin'] = array(
        '#type' => 'date_popup', // types 'date_text' and 'date_timezone' are also supported. See .inc file.
        '#timepicker' => 'timepicker',
        '#title' => t('Select a begin date'),
        '#default_value' => isset($filter_data['date_begin'][0]) ? $filter_data['date_begin'][0] : date("Y-m-d", strtotime("-1 month")),
        '#date_format' => $format,
        '#date_label_position' => 'within', // See other available attributes and what they do in date_api_elements.inc
        '#date_increment' => 15, // Optional, used by the date_select and date_popup elements to increment minutes and seconds.
        '#date_year_range' => '-3:+0', // Optional, used to set the year range (back 3 years and forward 3 years is the default).
    );
    $form['date_end'] = array(
        '#type' => 'date_popup', // types 'date_text' and 'date_timezone' are also supported. See .inc file.
        '#timepicker' => 'timepicker',
        '#title' => t('Select a end date'),
        '#default_value' => isset($filter_data['date_end'][0]) ? $filter_data['date_end'][0] : date("Y-m-d", time()),
        '#date_format' => $format,
        '#date_label_position' => 'within', // See other available attributes and what they do in date_api_elements.inc
        '#date_increment' => 15, // Optional, used by the date_select and date_popup elements to increment minutes and seconds.
        '#date_year_range' => '-3:+0', // Optional, used to set the year range (back 3 years and forward 3 years is the default).
    );
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

function gusev_analytics_pages_form_validate($form, &$form_state) {
    $date_begin = strtotime($form_state['values']['date_begin']);
    $date_end = strtotime($form_state['values']['date_end']);
    if ($date_begin > $date_end) {
        form_set_error('date_begin', 'Ошибка');
        $_SESSION['gusev_analytics_pages_form']['error'] = TRUE;
    }else{
        $_SESSION['gusev_analytics_pages_form']['error'] = FALSE;
    }
}

function gusev_analytics_pages_form_submit($form, &$form_state, $state) {
        $_SESSION['gusev_analytics_pages_form'] = array(
            'type' => array($form_state['values']['type']),
            'specialization' => array($form_state['values']['specialization']),
            'date_begin' => array($form_state['values']['date_begin']),
            'date_end' => array($form_state['values']['date_end'])
        );
}

function gusev_analytics_pages_form_xls_submit($form, &$form_state, $state) {
    $_SESSION['gusev_analytics_pages_form_xml'] = TRUE;
}
