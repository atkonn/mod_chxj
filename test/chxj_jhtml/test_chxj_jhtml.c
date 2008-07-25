#include <CUnit/CUnit.h>
#include <CUnit/Console.h>
#include <CUnit/Basic.h>
#include <stdio.h>
#define CHXJ_TEST
#define IMG_NOT_CONVERT_FILENAME

#include "chxj_apache.h"
#include "apr.h"
#include "qs_malloc.c"
#include "qs_log.c"
#include "qs_parse_tag.c"
#include "qs_parse_string.c"
#include "qs_parse_attr.c"
#include "qs_ignore_sp.c"
#include "qs_parse_file.c"
#include "chxj_buffered_write.c"
#include "chxj_chtml10.c"
#include "chxj_chtml20.c"
#include "chxj_chtml30.c"
#include "chxj_chtml40.c"
#include "chxj_chtml50.c"
#include "chxj_jhtml.c"
#include "chxj_jxhtml.c"
#include "chxj_hdml.c"
#include "chxj_xhtml_mobile_1_0.c"
#include "chxj_node_convert.c"
#include "chxj_tag_util.c"
#include "chxj_encoding.c"
#include "chxj_cookie.c"
#include "chxj_img_conv.c"
#include "chxj_url_encode.c"
#include "chxj_apache.c"
#include "chxj_dbm.c"
#include "chxj_str_util.c"
#include <iconv.h>
#include "chxj_serf.h"
#include "chxj_css.h"
#include "chxj_serf.c"
#include "chxj_css.c"



void test_jhtml_001();
void test_jhtml_002();
void test_jhtml_comment_tag_001();

void test_jhtml_a_tag_name_attribute_001();
void test_jhtml_a_tag_name_attribute_002();
void test_jhtml_a_tag_name_attribute_003();
void test_jhtml_a_tag_name_attribute_004();
void test_jhtml_a_tag_href_attribute_001();
void test_jhtml_a_tag_href_attribute_002();
void test_jhtml_a_tag_href_attribute_003();
void test_jhtml_a_tag_href_attribute_004();
void test_jhtml_a_tag_href_attribute_005();
void test_jhtml_a_tag_href_attribute_006();
void test_jhtml_a_tag_accesskey_attribute_001();
void test_jhtml_a_tag_accesskey_attribute_002();
void test_jhtml_a_tag_accesskey_attribute_003();

void test_jhtml_base_tag_001();
void test_jhtml_base_tag_href_attribute_001();
void test_jhtml_base_tag_href_attribute_002();
void test_jhtml_base_tag_href_attribute_003();
void test_jhtml_base_tag_href_attribute_004();

void test_jhtml_blockquote_tag_001();
void test_jhtml_blockquote_tag_002();
void test_jhtml_blockquote_tag_003();
void test_jhtml_blockquote_tag_004();

void test_jhtml_body_tag_001();
void test_jhtml_body_tag_002();
void test_jhtml_body_tag_003();
void test_jhtml_body_tag_004();
void test_jhtml_body_tag_005();
void test_jhtml_body_tag_006();
void test_jhtml_body_tag_007();
void test_jhtml_body_tag_008();
void test_jhtml_body_tag_009();
void test_jhtml_body_tag_010();
void test_jhtml_body_tag_011();
void test_jhtml_body_tag_012();
void test_jhtml_body_tag_013();
void test_jhtml_body_tag_014();
void test_jhtml_body_tag_015();

void test_jhtml_br_tag_001();
void test_jhtml_br_tag_002();
void test_jhtml_br_tag_003();
void test_jhtml_br_tag_004();
void test_jhtml_br_tag_005();
void test_jhtml_br_tag_006();
void test_jhtml_br_tag_007();

void test_jhtml_center_tag_001();

void test_jhtml_dir_tag_001();
void test_jhtml_dir_tag_002();
void test_jhtml_dir_tag_003();

void test_jhtml_dl_tag_001();

void test_jhtml_dt_tag_001();
void test_jhtml_dt_tag_002();

void test_jhtml_dd_tag_001();
void test_jhtml_dd_tag_002();

void test_jhtml_div_tag_001();
void test_jhtml_div_tag_002();
void test_jhtml_div_tag_003();
void test_jhtml_div_tag_004();
void test_jhtml_div_tag_005();
void test_jhtml_div_tag_006();
void test_jhtml_div_tag_007();

void test_jhtml_form_tag_001();
void test_jhtml_form_tag_002();
void test_jhtml_form_tag_003();
void test_jhtml_form_tag_004();
void test_jhtml_form_tag_005();
void test_jhtml_form_tag_006();
void test_jhtml_form_tag_007();
void test_jhtml_form_tag_008();
void test_jhtml_form_tag_009();

void test_jhtml_head_tag_001();
void test_jhtml_head_tag_002();

void test_jhtml_h1_tag_001(); 
void test_jhtml_h1_tag_002(); 
void test_jhtml_h1_tag_003(); 
void test_jhtml_h1_tag_004(); 
void test_jhtml_h1_tag_005(); 
void test_jhtml_h1_tag_006(); 
void test_jhtml_h1_tag_007(); 
void test_jhtml_h1_tag_008(); 
void test_jhtml_h1_tag_009(); 
void test_jhtml_h1_tag_010(); 

void test_jhtml_h2_tag_001(); 
void test_jhtml_h2_tag_002(); 
void test_jhtml_h2_tag_003(); 
void test_jhtml_h2_tag_004(); 
void test_jhtml_h2_tag_005(); 
void test_jhtml_h2_tag_006(); 
void test_jhtml_h2_tag_007(); 
void test_jhtml_h2_tag_008(); 
void test_jhtml_h2_tag_009(); 
void test_jhtml_h2_tag_010(); 

void test_jhtml_h3_tag_001(); 
void test_jhtml_h3_tag_002(); 
void test_jhtml_h3_tag_003(); 
void test_jhtml_h3_tag_004(); 
void test_jhtml_h3_tag_005(); 
void test_jhtml_h3_tag_006(); 
void test_jhtml_h3_tag_007(); 
void test_jhtml_h3_tag_008(); 
void test_jhtml_h3_tag_009(); 
void test_jhtml_h3_tag_010(); 

void test_jhtml_h4_tag_001(); 
void test_jhtml_h4_tag_002(); 
void test_jhtml_h4_tag_003(); 
void test_jhtml_h4_tag_004(); 
void test_jhtml_h4_tag_005(); 
void test_jhtml_h4_tag_006(); 
void test_jhtml_h4_tag_007(); 
void test_jhtml_h4_tag_008(); 
void test_jhtml_h4_tag_009(); 
void test_jhtml_h4_tag_010(); 

void test_jhtml_h5_tag_001(); 
void test_jhtml_h5_tag_002(); 
void test_jhtml_h5_tag_003(); 
void test_jhtml_h5_tag_004(); 
void test_jhtml_h5_tag_005(); 
void test_jhtml_h5_tag_006(); 
void test_jhtml_h5_tag_007(); 
void test_jhtml_h5_tag_008(); 
void test_jhtml_h5_tag_009(); 
void test_jhtml_h5_tag_010(); 

void test_jhtml_h6_tag_001(); 
void test_jhtml_h6_tag_002(); 
void test_jhtml_h6_tag_003(); 
void test_jhtml_h6_tag_004(); 
void test_jhtml_h6_tag_005(); 
void test_jhtml_h6_tag_006(); 
void test_jhtml_h6_tag_007(); 
void test_jhtml_h6_tag_008(); 
void test_jhtml_h6_tag_009(); 
void test_jhtml_h6_tag_010(); 

void test_jhtml_hr_tag_001();
void test_jhtml_hr_tag_002();
void test_jhtml_hr_tag_003();
void test_jhtml_hr_tag_004();
void test_jhtml_hr_tag_005();
void test_jhtml_hr_tag_006();
void test_jhtml_hr_tag_007();
void test_jhtml_hr_tag_008();
void test_jhtml_hr_tag_009();
void test_jhtml_hr_tag_010();
void test_jhtml_hr_tag_011();
void test_jhtml_hr_tag_012();
void test_jhtml_hr_tag_013();
void test_jhtml_hr_tag_014();
void test_jhtml_hr_tag_015();
void test_jhtml_hr_tag_016();
void test_jhtml_hr_tag_017();
void test_jhtml_hr_tag_018();
void test_jhtml_hr_tag_018_1();
void test_jhtml_hr_tag_018_2();

void test_jhtml_html_tag_001();

void test_jhtml_img_tag_001();
void test_jhtml_img_tag_002();
void test_jhtml_img_tag_003();
void test_jhtml_img_tag_004();
void test_jhtml_img_tag_005();
void test_jhtml_img_tag_006();
void test_jhtml_img_tag_007();
void test_jhtml_img_tag_008();
void test_jhtml_img_tag_009();
void test_jhtml_img_tag_010();
void test_jhtml_img_tag_011();
void test_jhtml_img_tag_012();
void test_jhtml_img_tag_013();
void test_jhtml_img_tag_013_1();
void test_jhtml_img_tag_014();
void test_jhtml_img_tag_015();
void test_jhtml_img_tag_016();
void test_jhtml_img_tag_017();
void test_jhtml_img_tag_018();
void test_jhtml_img_tag_019();
void test_jhtml_img_tag_020();
void test_jhtml_img_tag_021();
void test_jhtml_img_tag_022();
void test_jhtml_img_tag_023();
void test_jhtml_img_tag_024();
void test_jhtml_img_tag_025();
void test_jhtml_img_tag_026();
void test_jhtml_img_tag_027();
void test_jhtml_img_tag_028();
void test_jhtml_img_tag_029();
void test_jhtml_img_tag_030();
void test_jhtml_img_tag_031();
void test_jhtml_img_tag_032();
void test_jhtml_img_tag_033();
void test_jhtml_img_tag_034();
void test_jhtml_img_tag_035();
void test_jhtml_img_tag_036();
void test_jhtml_img_tag_037();

void test_jhtml_input_tag_001();
void test_jhtml_input_tag_002();
void test_jhtml_input_tag_003();
void test_jhtml_input_tag_004();
void test_jhtml_input_tag_005();
void test_jhtml_input_tag_006();
void test_jhtml_input_tag_007();
void test_jhtml_input_tag_008();
void test_jhtml_input_tag_009();
void test_jhtml_input_tag_010();
void test_jhtml_input_tag_011();
void test_jhtml_input_tag_012();
void test_jhtml_input_tag_013();
void test_jhtml_input_tag_014();
void test_jhtml_input_tag_015();
void test_jhtml_input_tag_016();
void test_jhtml_input_tag_017();
void test_jhtml_input_tag_018();
void test_jhtml_input_tag_019();
void test_jhtml_input_tag_020();
void test_jhtml_input_tag_021();
void test_jhtml_input_tag_022();
void test_jhtml_input_tag_023();
void test_jhtml_input_tag_024();
void test_jhtml_input_tag_025();
void test_jhtml_input_tag_026();
void test_jhtml_input_tag_027();
void test_jhtml_input_tag_028();
void test_jhtml_input_tag_029();
void test_jhtml_input_tag_030();
void test_jhtml_input_tag_031();
void test_jhtml_input_tag_032();
void test_jhtml_input_tag_033();
void test_jhtml_input_tag_034();
void test_jhtml_input_tag_035();
void test_jhtml_input_tag_036();
void test_jhtml_input_tag_037();
void test_jhtml_input_tag_038();
void test_jhtml_input_tag_039();
void test_jhtml_input_tag_040();
void test_jhtml_input_tag_041();
void test_jhtml_input_tag_042();
void test_jhtml_input_tag_043();
void test_jhtml_input_tag_044();
void test_jhtml_input_tag_045();
void test_jhtml_input_tag_046();
void test_jhtml_input_tag_047();

void test_jhtml_li_tag_001();
void test_jhtml_li_tag_002();
void test_jhtml_li_tag_003();
void test_jhtml_li_tag_004();
void test_jhtml_li_tag_005();
void test_jhtml_li_tag_006();
void test_jhtml_li_tag_007();
void test_jhtml_li_tag_008();
void test_jhtml_li_tag_009();
void test_jhtml_li_tag_010();
void test_jhtml_li_tag_011();
void test_jhtml_li_tag_012();
void test_jhtml_li_tag_013();
void test_jhtml_li_tag_014();
void test_jhtml_li_tag_015();
void test_jhtml_li_tag_016();

void test_jhtml_menu_tag_001();
void test_jhtml_menu_tag_002();
void test_jhtml_menu_tag_003();
void test_jhtml_menu_tag_004();
void test_jhtml_menu_tag_005();
void test_jhtml_menu_tag_006();

void test_jhtml_ol_tag_001();
void test_jhtml_ol_tag_002();
void test_jhtml_ol_tag_003();
void test_jhtml_ol_tag_004();
void test_jhtml_ol_tag_005();
void test_jhtml_ol_tag_006();
void test_jhtml_ol_tag_007();
void test_jhtml_ol_tag_008();
void test_jhtml_ol_tag_009();
void test_jhtml_ol_tag_010();
void test_jhtml_ol_tag_011();
void test_jhtml_ol_tag_012();
void test_jhtml_ol_tag_013();
void test_jhtml_ol_tag_014();
void test_jhtml_ol_tag_015();
void test_jhtml_ol_tag_016();
void test_jhtml_ol_tag_017();
void test_jhtml_ol_tag_018();

void test_jhtml_option_tag_001();
void test_jhtml_option_tag_002();
void test_jhtml_option_tag_003();
void test_jhtml_option_tag_004();
void test_jhtml_option_tag_005();
void test_jhtml_option_tag_006();
void test_jhtml_option_tag_007();

void test_jhtml_p_tag_001();
void test_jhtml_p_tag_002();
void test_jhtml_p_tag_003();
void test_jhtml_p_tag_004();
void test_jhtml_p_tag_005();
void test_jhtml_p_tag_006();
void test_jhtml_p_tag_007();
void test_jhtml_p_tag_008();

void test_jhtml_plaintext_tag_001();
void test_jhtml_plaintext_tag_002();
void test_jhtml_plaintext_tag_003();
void test_jhtml_plaintext_tag_004();

void test_jhtml_pre_tag_001();
void test_jhtml_pre_tag_002();
void test_jhtml_pre_tag_003();
void test_jhtml_pre_tag_004();
void test_jhtml_pre_tag_005();

void test_jhtml_select_tag_001();
void test_jhtml_select_tag_002();
void test_jhtml_select_tag_003();
void test_jhtml_select_tag_004();
void test_jhtml_select_tag_005();
void test_jhtml_select_tag_006();
void test_jhtml_select_tag_007();
void test_jhtml_select_tag_008();
void test_jhtml_select_tag_009();
void test_jhtml_select_tag_010();
void test_jhtml_select_tag_011();
void test_jhtml_select_tag_012();
void test_jhtml_select_tag_013();
void test_jhtml_select_tag_014();

void test_jhtml_textarea_tag_001();
void test_jhtml_textarea_tag_002();
void test_jhtml_textarea_tag_003();
void test_jhtml_textarea_tag_004();
void test_jhtml_textarea_tag_005();
void test_jhtml_textarea_tag_006();
void test_jhtml_textarea_tag_007();
void test_jhtml_textarea_tag_008();
void test_jhtml_textarea_tag_009();
void test_jhtml_textarea_tag_010();
void test_jhtml_textarea_tag_011();
void test_jhtml_textarea_tag_012();
void test_jhtml_textarea_tag_013();
void test_jhtml_textarea_tag_014();
void test_jhtml_textarea_tag_015();
void test_jhtml_textarea_tag_016();
void test_jhtml_textarea_tag_017();
void test_jhtml_textarea_tag_018();
void test_jhtml_textarea_tag_019();
void test_jhtml_textarea_tag_020();
void test_jhtml_textarea_tag_021();
void test_jhtml_textarea_tag_022();
void test_jhtml_textarea_tag_023();
void test_jhtml_textarea_tag_024();
void test_jhtml_textarea_tag_025();
void test_jhtml_textarea_tag_026();

void test_jhtml_title_tag_001();
void test_jhtml_title_tag_002();

void test_jhtml_ul_tag_001();
void test_jhtml_ul_tag_002();
void test_jhtml_ul_tag_003();
void test_jhtml_ul_tag_004();
void test_jhtml_ul_tag_005();
void test_jhtml_ul_tag_006();

void test_jhtml_blink_tag_001();
void test_jhtml_blink_tag_002();
void test_jhtml_blink_tag_003();
void test_jhtml_blink_tag_004();

void test_jhtml_marquee_tag_001();
void test_jhtml_marquee_tag_002();
void test_jhtml_marquee_tag_003();
void test_jhtml_marquee_tag_004();
void test_jhtml_marquee_tag_005();
void test_jhtml_marquee_tag_006();
void test_jhtml_marquee_tag_007();
void test_jhtml_marquee_tag_008();
void test_jhtml_marquee_tag_009();
void test_jhtml_marquee_tag_010();
void test_jhtml_marquee_tag_011();
void test_jhtml_marquee_tag_012();
void test_jhtml_marquee_tag_013();
void test_jhtml_marquee_tag_014();
void test_jhtml_marquee_tag_015();
void test_jhtml_marquee_tag_016();
void test_jhtml_marquee_tag_017();
void test_jhtml_marquee_tag_018();
void test_jhtml_marquee_tag_019();
void test_jhtml_marquee_tag_020();
void test_jhtml_marquee_tag_021();

void test_jhtml_meta_tag_001();
void test_jhtml_meta_tag_002();
void test_jhtml_meta_tag_003();
void test_jhtml_meta_tag_004();
void test_jhtml_meta_tag_005();
void test_jhtml_meta_tag_006();
void test_jhtml_meta_tag_007();
void test_jhtml_meta_tag_008();
void test_jhtml_meta_tag_009();

void test_jhtml_font_tag_001();
void test_jhtml_font_tag_002();
void test_jhtml_font_tag_003();
void test_jhtml_font_tag_004();
void test_jhtml_font_tag_005();
void test_jhtml_font_tag_006();
void test_jhtml_font_tag_007();
void test_jhtml_font_tag_008();
void test_jhtml_font_tag_009();
void test_jhtml_font_tag_010();

void test_jhtml_param_tag_001();

/* CSS */
void test_jhtml_link_001();
void test_jhtml_link_002();
void test_jhtml_link_003();
void test_jhtml_link_004();
void test_jhtml_link_005();
void test_jhtml_link_006();
void test_jhtml_link_007();
void test_jhtml_link_008();
void test_jhtml_link_009();

void test_jhtml_html_tag_with_css_001();
void test_jhtml_meta_tag_with_css_001();

void test_jhtml_textarea_tag_with_css_001();
void test_jhtml_textarea_tag_with_css_002();
void test_jhtml_textarea_tag_with_css_003();
void test_jhtml_textarea_tag_with_css_004();

void test_jhtml_textarea_tag_with_css_005();
void test_jhtml_textarea_tag_with_css_006();
void test_jhtml_textarea_tag_with_css_007();
void test_jhtml_textarea_tag_with_css_008();

void test_jhtml_p_tag_with_css_001();
void test_jhtml_p_tag_with_css_002();
void test_jhtml_p_tag_with_css_003();
void test_jhtml_p_tag_with_css_004();
void test_jhtml_p_tag_with_css_005();
void test_jhtml_p_tag_with_css_006();
void test_jhtml_p_tag_with_css_007();
void test_jhtml_p_tag_with_css_008();
void test_jhtml_p_tag_with_css_009();
void test_jhtml_p_tag_with_css_010();
void test_jhtml_p_tag_with_css_011();
void test_jhtml_p_tag_with_css_012();
void test_jhtml_p_tag_with_css_013();

void test_jhtml_li_tag_with_css_001();
void test_jhtml_li_tag_with_css_002();
void test_jhtml_li_tag_with_css_003();
void test_jhtml_li_tag_with_css_004();
void test_jhtml_li_tag_with_css_005();
void test_jhtml_li_tag_with_css_006();
void test_jhtml_li_tag_with_css_007();
void test_jhtml_li_tag_with_css_008();
void test_jhtml_li_tag_with_css_009();
void test_jhtml_li_tag_with_css_010();
void test_jhtml_li_tag_with_css_011();
void test_jhtml_li_tag_with_css_012();

void test_jhtml_ol_tag_with_css_001();
void test_jhtml_ol_tag_with_css_002();
void test_jhtml_ol_tag_with_css_003();
void test_jhtml_ol_tag_with_css_004();
void test_jhtml_ol_tag_with_css_005();
void test_jhtml_ol_tag_with_css_006();

void test_jhtml_h1_tag_with_css_001();
void test_jhtml_h1_tag_with_css_002();
void test_jhtml_h1_tag_with_css_003();
void test_jhtml_h1_tag_with_css_004();
void test_jhtml_h1_tag_with_css_005();
void test_jhtml_h1_tag_with_css_006();

void test_jhtml_h2_tag_with_css_001();
void test_jhtml_h2_tag_with_css_002();
void test_jhtml_h2_tag_with_css_003();
void test_jhtml_h2_tag_with_css_004();
void test_jhtml_h2_tag_with_css_005();
void test_jhtml_h2_tag_with_css_006();

void test_jhtml_h3_tag_with_css_001();
void test_jhtml_h3_tag_with_css_002();
void test_jhtml_h3_tag_with_css_003();
void test_jhtml_h3_tag_with_css_004();
void test_jhtml_h3_tag_with_css_005();
void test_jhtml_h3_tag_with_css_006();

void test_jhtml_h4_tag_with_css_001();
void test_jhtml_h4_tag_with_css_002();
void test_jhtml_h4_tag_with_css_003();
void test_jhtml_h4_tag_with_css_004();
void test_jhtml_h4_tag_with_css_005();
void test_jhtml_h4_tag_with_css_006();

void test_jhtml_h5_tag_with_css_001();
void test_jhtml_h5_tag_with_css_002();
void test_jhtml_h5_tag_with_css_003();
void test_jhtml_h5_tag_with_css_004();
void test_jhtml_h5_tag_with_css_005();
void test_jhtml_h5_tag_with_css_006();

void test_jhtml_h6_tag_with_css_001();
void test_jhtml_h6_tag_with_css_002();
void test_jhtml_h6_tag_with_css_003();
void test_jhtml_h6_tag_with_css_004();
void test_jhtml_h6_tag_with_css_005();
void test_jhtml_h6_tag_with_css_006();

void test_jhtml_body_tag_with_css_001();
void test_jhtml_body_tag_with_css_002();
void test_jhtml_body_tag_with_css_003();
void test_jhtml_body_tag_with_css_004();
void test_jhtml_body_tag_with_css_005();
void test_jhtml_body_tag_with_css_006();

void test_jhtml_font_tag_with_css_001();
void test_jhtml_font_tag_with_css_002();

void test_jhtml_form_tag_with_css_001();
void test_jhtml_form_tag_with_css_002();
void test_jhtml_form_tag_with_css_003();
void test_jhtml_form_tag_with_css_004();
void test_jhtml_form_tag_with_css_005();
void test_jhtml_form_tag_with_css_006();
void test_jhtml_form_tag_with_css_007();
void test_jhtml_form_tag_with_css_008();

void test_jhtml_input_tag_with_css_001();
void test_jhtml_input_tag_with_css_002();
void test_jhtml_input_tag_with_css_003();
void test_jhtml_input_tag_with_css_004();
void test_jhtml_input_tag_with_css_005();
void test_jhtml_input_tag_with_css_006();
void test_jhtml_input_tag_with_css_007();
void test_jhtml_input_tag_with_css_008();

void test_jhtml_center_tag_with_css_001();
void test_jhtml_center_tag_with_css_002();
void test_jhtml_center_tag_with_css_003();
void test_jhtml_center_tag_with_css_004();

void test_jhtml_hr_tag_with_css_001();
void test_jhtml_hr_tag_with_css_002();
void test_jhtml_hr_tag_with_css_003();
void test_jhtml_hr_tag_with_css_004();
void test_jhtml_hr_tag_with_css_005();
void test_jhtml_hr_tag_with_css_006();
void test_jhtml_hr_tag_with_css_007();
void test_jhtml_hr_tag_with_css_008();

void test_jhtml_img_tag_with_css_001();
void test_jhtml_img_tag_with_css_002();
void test_jhtml_img_tag_with_css_003();
void test_jhtml_img_tag_with_css_004();
void test_jhtml_img_tag_with_css_005();
void test_jhtml_img_tag_with_css_006();
void test_jhtml_img_tag_with_css_007();
void test_jhtml_img_tag_with_css_008();
void test_jhtml_img_tag_with_css_009();
void test_jhtml_img_tag_with_css_010();


void test_jhtml_div_tag_with_css_001();
void test_jhtml_div_tag_with_css_002();
void test_jhtml_div_tag_with_css_003();
void test_jhtml_div_tag_with_css_004();
void test_jhtml_div_tag_with_css_005();
void test_jhtml_div_tag_with_css_006();
void test_jhtml_div_tag_with_css_007();
void test_jhtml_div_tag_with_css_008();
void test_jhtml_div_tag_with_css_009();
void test_jhtml_div_tag_with_css_010();
void test_jhtml_div_tag_with_css_011();
void test_jhtml_div_tag_with_css_012();
void test_jhtml_div_tag_with_css_013();
void test_jhtml_div_tag_with_css_014();
void test_jhtml_div_tag_with_css_015();
void test_jhtml_div_tag_with_css_016();
void test_jhtml_div_tag_with_css_017();
void test_jhtml_div_tag_with_css_018();
void test_jhtml_div_tag_with_css_019();
void test_jhtml_div_tag_with_css_020();
void test_jhtml_div_tag_with_css_021();
void test_jhtml_div_tag_with_css_022();
void test_jhtml_div_tag_with_css_023();
void test_jhtml_div_tag_with_css_024();
void test_jhtml_div_tag_with_css_025();
void test_jhtml_div_tag_with_css_026();
void test_jhtml_div_tag_with_css_027();
void test_jhtml_div_tag_with_css_028();
void test_jhtml_div_tag_with_css_029();
void test_jhtml_div_tag_with_css_030();
void test_jhtml_div_tag_with_css_031();
void test_jhtml_div_tag_with_css_032();
void test_jhtml_div_tag_with_css_033();
void test_jhtml_div_tag_with_css_034();
void test_jhtml_div_tag_with_css_035();
void test_jhtml_div_tag_with_css_036();
void test_jhtml_div_tag_with_css_037();
void test_jhtml_div_tag_with_css_038();
void test_jhtml_div_tag_with_css_039();
void test_jhtml_div_tag_with_css_040();
void test_jhtml_div_tag_with_css_041();
void test_jhtml_div_tag_with_css_042();

void test_jhtml_dt_tag_with_css_001();
void test_jhtml_dt_tag_with_css_002();
void test_jhtml_dt_tag_with_css_003();
void test_jhtml_dt_tag_with_css_004();
void test_jhtml_dt_tag_with_css_005();
void test_jhtml_dt_tag_with_css_006();
void test_jhtml_dt_tag_with_css_007();
void test_jhtml_dt_tag_with_css_008();
void test_jhtml_dt_tag_with_css_009();
void test_jhtml_dt_tag_with_css_010();
void test_jhtml_dt_tag_with_css_011();
void test_jhtml_dt_tag_with_css_012();
void test_jhtml_dt_tag_with_css_013();
void test_jhtml_dt_tag_with_css_014();

void test_jhtml_blockquote_tag_with_css_001();
void test_jhtml_blockquote_tag_with_css_002();
void test_jhtml_blockquote_tag_with_css_003();
void test_jhtml_blockquote_tag_with_css_004();
void test_jhtml_blockquote_tag_with_css_005();
void test_jhtml_blockquote_tag_with_css_006();
void test_jhtml_blockquote_tag_with_css_007();
void test_jhtml_blockquote_tag_with_css_008();
void test_jhtml_blockquote_tag_with_css_009();
void test_jhtml_blockquote_tag_with_css_010();
void test_jhtml_blockquote_tag_with_css_011();
void test_jhtml_blockquote_tag_with_css_012();
void test_jhtml_blockquote_tag_with_css_013();
void test_jhtml_blockquote_tag_with_css_014();
void test_jhtml_blockquote_tag_with_css_015();
void test_jhtml_blockquote_tag_with_css_016();


void test_jhtml_dir_tag_with_css_001();
void test_jhtml_dir_tag_with_css_002();
void test_jhtml_dir_tag_with_css_003();
void test_jhtml_dir_tag_with_css_004();
void test_jhtml_dir_tag_with_css_005();
void test_jhtml_dir_tag_with_css_006();
void test_jhtml_dir_tag_with_css_007();
void test_jhtml_dir_tag_with_css_008();
void test_jhtml_dir_tag_with_css_009();
void test_jhtml_dir_tag_with_css_010();
void test_jhtml_dir_tag_with_css_011();
void test_jhtml_dir_tag_with_css_012();
void test_jhtml_dir_tag_with_css_013();
void test_jhtml_dir_tag_with_css_014();
void test_jhtml_dir_tag_with_css_015();
void test_jhtml_dir_tag_with_css_016();
void test_jhtml_dir_tag_with_css_017();
void test_jhtml_dir_tag_with_css_018();
void test_jhtml_dir_tag_with_css_019();
void test_jhtml_dir_tag_with_css_020();
void test_jhtml_dir_tag_with_css_021();
void test_jhtml_dir_tag_with_css_022();
/* pend */

int
main()
{
  CU_pSuite jhtml_suite;
  CU_initialize_registry();
  jhtml_suite = CU_add_suite("test chxj_convert_jhtml()", NULL, NULL);

  /*=========================================================================*/
  /* <!-- -->                                                                */
  /*=========================================================================*/
  CU_add_test(jhtml_suite, "test void src1",                                    test_jhtml_001);
  CU_add_test(jhtml_suite, "test void src2",                                    test_jhtml_002);
  CU_add_test(jhtml_suite, "test comment tag1",                                 test_jhtml_comment_tag_001);

  /*=========================================================================*/
  /* <A>                                                                     */
  /*=========================================================================*/
  CU_add_test(jhtml_suite, "test a tag name attr1",                             test_jhtml_a_tag_name_attribute_001);
  CU_add_test(jhtml_suite, "test a tag name attr2",                             test_jhtml_a_tag_name_attribute_002);
  CU_add_test(jhtml_suite, "test a tag name attr3 with japanese.",              test_jhtml_a_tag_name_attribute_003);
  CU_add_test(jhtml_suite, "test a tag name attr4 with japanese.",              test_jhtml_a_tag_name_attribute_004);
  CU_add_test(jhtml_suite, "test a tag href attr1 with void attribute.",        test_jhtml_a_tag_href_attribute_001);
  CU_add_test(jhtml_suite, "test a tag href attr2 with other site link.",       test_jhtml_a_tag_href_attribute_002);
  CU_add_test(jhtml_suite, "test a tag href attr3 with local link.",            test_jhtml_a_tag_href_attribute_003);
  CU_add_test(jhtml_suite, "test a tag href attr4 with maker.",                 test_jhtml_a_tag_href_attribute_004);
  CU_add_test(jhtml_suite, "test a tag href attr5 with void maker.",            test_jhtml_a_tag_href_attribute_005);
  CU_add_test(jhtml_suite, "test a tag href attr6 with no cookie.",             test_jhtml_a_tag_href_attribute_006);
  CU_add_test(jhtml_suite, "test a tag accesskey attribute.",                   test_jhtml_a_tag_accesskey_attribute_001);
  CU_add_test(jhtml_suite, "test a tag accesskey attribute with void char.",    test_jhtml_a_tag_accesskey_attribute_002);
  CU_add_test(jhtml_suite, "test a tag accesskey attribute with no value",      test_jhtml_a_tag_accesskey_attribute_003);

  /*=========================================================================*/
  /* <BASE>                                                                  */
  /*=========================================================================*/
  CU_add_test(jhtml_suite, "test base tag no attribute.",                       test_jhtml_base_tag_001);
  CU_add_test(jhtml_suite, "test base tag href attribute with no value.",       test_jhtml_base_tag_href_attribute_001);
  CU_add_test(jhtml_suite, "test base tag href attribute with void value.",     test_jhtml_base_tag_href_attribute_002);
  CU_add_test(jhtml_suite, "test base tag href attribute with normal value.",   test_jhtml_base_tag_href_attribute_003);
  CU_add_test(jhtml_suite, "test base tag href attribute with normal value.",   test_jhtml_base_tag_href_attribute_004);

  /*=========================================================================*/
  /* <BLOCKQUOTE>                                                            */
  /*=========================================================================*/
  CU_add_test(jhtml_suite, "test <blockquote> with void value.",                test_jhtml_blockquote_tag_001);
  CU_add_test(jhtml_suite, "test <blockquote> with value.",                     test_jhtml_blockquote_tag_002);
  CU_add_test(jhtml_suite, "test <blockquote> with japanese value.",            test_jhtml_blockquote_tag_003);
  CU_add_test(jhtml_suite, "test <blockquote> with hankaku kana value.",        test_jhtml_blockquote_tag_004);

  /*=========================================================================*/
  /* <BODY>                                                                  */
  /*=========================================================================*/
  CU_add_test(jhtml_suite, "test <body> .",                                     test_jhtml_body_tag_001);
  CU_add_test(jhtml_suite, "test <body> with bgcolor attribute 1.",             test_jhtml_body_tag_002);
  CU_add_test(jhtml_suite, "test <body> with bgcolor attribute 2.",             test_jhtml_body_tag_003);
  CU_add_test(jhtml_suite, "test <body> with bgcolor attribute 3.",             test_jhtml_body_tag_004);
  CU_add_test(jhtml_suite, "test <body> with text attribute 1.",                test_jhtml_body_tag_005);
  CU_add_test(jhtml_suite, "test <body> with text attribute 2.",                test_jhtml_body_tag_006);
  CU_add_test(jhtml_suite, "test <body> with text attribute 3.",                test_jhtml_body_tag_007);
  CU_add_test(jhtml_suite, "test <body> with link attribute 1.",                test_jhtml_body_tag_008);
  CU_add_test(jhtml_suite, "test <body> with link attribute 2.",                test_jhtml_body_tag_009);
  CU_add_test(jhtml_suite, "test <body> with link attribute 3.",                test_jhtml_body_tag_010);
  CU_add_test(jhtml_suite, "test <body> with vlink attribute 1.",               test_jhtml_body_tag_011);
  CU_add_test(jhtml_suite, "test <body> with vlink attribute 2.",               test_jhtml_body_tag_012);
  CU_add_test(jhtml_suite, "test <body> with vlink attribute 3.",               test_jhtml_body_tag_013);
  CU_add_test(jhtml_suite, "test <body> with alink attribute.",                 test_jhtml_body_tag_014);
  CU_add_test(jhtml_suite, "test <body> with unknown attribute.",               test_jhtml_body_tag_015);

  /*=========================================================================*/
  /* <BR>                                                                    */
  /*=========================================================================*/
  CU_add_test(jhtml_suite, "test <br>.",                                        test_jhtml_br_tag_001);
  CU_add_test(jhtml_suite, "test <br> with clear attribute(left).",             test_jhtml_br_tag_002);
  CU_add_test(jhtml_suite, "test <br> with clear attribute(right).",            test_jhtml_br_tag_003);
  CU_add_test(jhtml_suite, "test <br> with clear attribute(all).",              test_jhtml_br_tag_004);
  CU_add_test(jhtml_suite, "test <br> with clear attribute(void).",             test_jhtml_br_tag_005);
  CU_add_test(jhtml_suite, "test <br> with clear attribute(no value).",         test_jhtml_br_tag_006);
  CU_add_test(jhtml_suite, "test <br> with clear attribute(unknown value).",    test_jhtml_br_tag_007);

  /*=========================================================================*/
  /* <CENTER>                                                                */
  /*=========================================================================*/
  CU_add_test(jhtml_suite, "test <center>.",                                    test_jhtml_center_tag_001);

  /*=========================================================================*/
  /* <DIR>                                                                   */
  /*=========================================================================*/
  CU_add_test(jhtml_suite, "test <dir>.",                                       test_jhtml_dir_tag_001);
  CU_add_test(jhtml_suite, "test <dir> with no <li>.",                          test_jhtml_dir_tag_002);
  CU_add_test(jhtml_suite, "test <dir type>.",                                  test_jhtml_dir_tag_003);

  /*=========================================================================*/
  /* <DL>                                                                    */
  /*=========================================================================*/
  CU_add_test(jhtml_suite, "test <dl>.",                                        test_jhtml_dl_tag_001);

  /*=========================================================================*/
  /* <DT>                                                                    */
  /*=========================================================================*/
  CU_add_test(jhtml_suite, "test <dt>.",                                        test_jhtml_dt_tag_001);
  CU_add_test(jhtml_suite, "test <dt> with void value.",                        test_jhtml_dt_tag_002);

  /*=========================================================================*/
  /* <DD>                                                                    */
  /*=========================================================================*/
  CU_add_test(jhtml_suite, "test <dd>.",                                        test_jhtml_dd_tag_001);
  CU_add_test(jhtml_suite, "test <dd> with void value.",                        test_jhtml_dd_tag_002);

  /*=========================================================================*/
  /* <DIV>                                                                   */
  /*=========================================================================*/
  CU_add_test(jhtml_suite, "test <div>.",                                       test_jhtml_div_tag_001);
  CU_add_test(jhtml_suite, "test <div> with align attribute(left).",            test_jhtml_div_tag_002);
  CU_add_test(jhtml_suite, "test <div> with align attribute(right).",           test_jhtml_div_tag_003);
  CU_add_test(jhtml_suite, "test <div> with align attribute(center).",          test_jhtml_div_tag_004);
  CU_add_test(jhtml_suite, "test <div> with align attribute(void).",            test_jhtml_div_tag_005);
  CU_add_test(jhtml_suite, "test <div> with align attribute(unknown).",         test_jhtml_div_tag_006);
  CU_add_test(jhtml_suite, "test <div> with style attribute.",                  test_jhtml_div_tag_007);

  /*=========================================================================*/
  /* <FORM>                                                                  */
  /*=========================================================================*/
  CU_add_test(jhtml_suite, "test <form>.",                                      test_jhtml_form_tag_001);
  CU_add_test(jhtml_suite, "test <form method>.",                               test_jhtml_form_tag_002);
  CU_add_test(jhtml_suite, "test <form method=\"post\">.",                      test_jhtml_form_tag_003);
  CU_add_test(jhtml_suite, "test <form method=\"get\">.",                       test_jhtml_form_tag_004);
  CU_add_test(jhtml_suite, "test <form method=\"abc\">.",                       test_jhtml_form_tag_005);
  CU_add_test(jhtml_suite, "test <form action>.",                               test_jhtml_form_tag_006);
  CU_add_test(jhtml_suite, "test <form action> with null cookie.",              test_jhtml_form_tag_007);
  CU_add_test(jhtml_suite, "test <form action> with other site .",              test_jhtml_form_tag_008);
  CU_add_test(jhtml_suite, "test <form action method>.",                        test_jhtml_form_tag_009);

  /*=========================================================================*/
  /* <HEAD>                                                                  */
  /*=========================================================================*/
  CU_add_test(jhtml_suite, "test <head>.",                                      test_jhtml_head_tag_001);
  CU_add_test(jhtml_suite, "test <head> with value.",                           test_jhtml_head_tag_002);

  /*=========================================================================*/
  /* <H1>                                                                    */
  /*=========================================================================*/
  CU_add_test(jhtml_suite, "test <h1>.",                                        test_jhtml_h1_tag_001);
  CU_add_test(jhtml_suite, "test <h1> with value.",                             test_jhtml_h1_tag_002);
  CU_add_test(jhtml_suite, "test <h1> with value(japanese).",                   test_jhtml_h1_tag_003);
  CU_add_test(jhtml_suite, "test <h1> with value(japanese-hankaku).",           test_jhtml_h1_tag_004);
  CU_add_test(jhtml_suite, "test <h1> with align attribute(non).",              test_jhtml_h1_tag_005);
  CU_add_test(jhtml_suite, "test <h1> with align attribute(void).",             test_jhtml_h1_tag_006);
  CU_add_test(jhtml_suite, "test <h1> with align attribute(left).",             test_jhtml_h1_tag_007);
  CU_add_test(jhtml_suite, "test <h1> with align attribute(right).",            test_jhtml_h1_tag_008);
  CU_add_test(jhtml_suite, "test <h1> with align attribute(center).",           test_jhtml_h1_tag_009);
  CU_add_test(jhtml_suite, "test <h1> with align attribute(unkown).",           test_jhtml_h1_tag_010);
  /*=========================================================================*/
  /* <H2>                                                                    */
  /*=========================================================================*/
  CU_add_test(jhtml_suite, "test <h2>.",                                        test_jhtml_h2_tag_001);
  CU_add_test(jhtml_suite, "test <h2> with value.",                             test_jhtml_h2_tag_002);
  CU_add_test(jhtml_suite, "test <h2> with value(japanese).",                   test_jhtml_h2_tag_003);
  CU_add_test(jhtml_suite, "test <h2> with value(japanese-hankaku).",           test_jhtml_h2_tag_004);
  CU_add_test(jhtml_suite, "test <h2> with align attribute(non).",              test_jhtml_h2_tag_005);
  CU_add_test(jhtml_suite, "test <h2> with align attribute(void).",             test_jhtml_h2_tag_006);
  CU_add_test(jhtml_suite, "test <h2> with align attribute(left).",             test_jhtml_h2_tag_007);
  CU_add_test(jhtml_suite, "test <h2> with align attribute(right).",            test_jhtml_h2_tag_008);
  CU_add_test(jhtml_suite, "test <h2> with align attribute(center).",           test_jhtml_h2_tag_009);
  CU_add_test(jhtml_suite, "test <h2> with align attribute(unkown).",           test_jhtml_h2_tag_010);
  /*=========================================================================*/
  /* <H3>                                                                    */
  /*=========================================================================*/
  CU_add_test(jhtml_suite, "test <h3>.",                                        test_jhtml_h3_tag_001);
  CU_add_test(jhtml_suite, "test <h3> with value.",                             test_jhtml_h3_tag_002);
  CU_add_test(jhtml_suite, "test <h3> with value(japanese).",                   test_jhtml_h3_tag_003);
  CU_add_test(jhtml_suite, "test <h3> with value(japanese-hankaku).",           test_jhtml_h3_tag_004);
  CU_add_test(jhtml_suite, "test <h3> with align attribute(non).",              test_jhtml_h3_tag_005);
  CU_add_test(jhtml_suite, "test <h3> with align attribute(void).",             test_jhtml_h3_tag_006);
  CU_add_test(jhtml_suite, "test <h3> with align attribute(left).",             test_jhtml_h3_tag_007);
  CU_add_test(jhtml_suite, "test <h3> with align attribute(right).",            test_jhtml_h3_tag_008);
  CU_add_test(jhtml_suite, "test <h3> with align attribute(center).",           test_jhtml_h3_tag_009);
  CU_add_test(jhtml_suite, "test <h3> with align attribute(unkown).",           test_jhtml_h3_tag_010);
  /*=========================================================================*/
  /* <H4>                                                                    */
  /*=========================================================================*/
  CU_add_test(jhtml_suite, "test <h4>.",                                        test_jhtml_h4_tag_001);
  CU_add_test(jhtml_suite, "test <h4> with value.",                             test_jhtml_h4_tag_002);
  CU_add_test(jhtml_suite, "test <h4> with value(japanese).",                   test_jhtml_h4_tag_003);
  CU_add_test(jhtml_suite, "test <h4> with value(japanese-hankaku).",           test_jhtml_h4_tag_004);
  CU_add_test(jhtml_suite, "test <h4> with align attribute(non).",              test_jhtml_h4_tag_005);
  CU_add_test(jhtml_suite, "test <h4> with align attribute(void).",             test_jhtml_h4_tag_006);
  CU_add_test(jhtml_suite, "test <h4> with align attribute(left).",             test_jhtml_h4_tag_007);
  CU_add_test(jhtml_suite, "test <h4> with align attribute(right).",            test_jhtml_h4_tag_008);
  CU_add_test(jhtml_suite, "test <h4> with align attribute(center).",           test_jhtml_h4_tag_009);
  CU_add_test(jhtml_suite, "test <h4> with align attribute(unkown).",           test_jhtml_h4_tag_010);
  /*=========================================================================*/
  /* <H5>                                                                    */
  /*=========================================================================*/
  CU_add_test(jhtml_suite, "test <h5>.",                                        test_jhtml_h5_tag_001);
  CU_add_test(jhtml_suite, "test <h5> with value.",                             test_jhtml_h5_tag_002);
  CU_add_test(jhtml_suite, "test <h5> with value(japanese).",                   test_jhtml_h5_tag_003);
  CU_add_test(jhtml_suite, "test <h5> with value(japanese-hankaku).",           test_jhtml_h5_tag_004);
  CU_add_test(jhtml_suite, "test <h5> with align attribute(non).",              test_jhtml_h5_tag_005);
  CU_add_test(jhtml_suite, "test <h5> with align attribute(void).",             test_jhtml_h5_tag_006);
  CU_add_test(jhtml_suite, "test <h5> with align attribute(left).",             test_jhtml_h5_tag_007);
  CU_add_test(jhtml_suite, "test <h5> with align attribute(right).",            test_jhtml_h5_tag_008);
  CU_add_test(jhtml_suite, "test <h5> with align attribute(center).",           test_jhtml_h5_tag_009);
  CU_add_test(jhtml_suite, "test <h5> with align attribute(unkown).",           test_jhtml_h5_tag_010);
  /*=========================================================================*/
  /* <H6>                                                                    */
  /*=========================================================================*/
  CU_add_test(jhtml_suite, "test <h6>.",                                        test_jhtml_h6_tag_001);
  CU_add_test(jhtml_suite, "test <h6> with value.",                             test_jhtml_h6_tag_002);
  CU_add_test(jhtml_suite, "test <h6> with value(japanese).",                   test_jhtml_h6_tag_003);
  CU_add_test(jhtml_suite, "test <h6> with value(japanese-hankaku).",           test_jhtml_h6_tag_004);
  CU_add_test(jhtml_suite, "test <h6> with align attribute(non).",              test_jhtml_h6_tag_005);
  CU_add_test(jhtml_suite, "test <h6> with align attribute(void).",             test_jhtml_h6_tag_006);
  CU_add_test(jhtml_suite, "test <h6> with align attribute(left).",             test_jhtml_h6_tag_007);
  CU_add_test(jhtml_suite, "test <h6> with align attribute(right).",            test_jhtml_h6_tag_008);
  CU_add_test(jhtml_suite, "test <h6> with align attribute(center).",           test_jhtml_h6_tag_009);
  CU_add_test(jhtml_suite, "test <h6> with align attribute(unkown).",           test_jhtml_h6_tag_010);

  /*=========================================================================*/
  /* <HR>                                                                    */
  /*=========================================================================*/
  CU_add_test(jhtml_suite, "test <hr>.",                                        test_jhtml_hr_tag_001);
  CU_add_test(jhtml_suite, "test <hr />.",                                      test_jhtml_hr_tag_002);
  CU_add_test(jhtml_suite, "test <hr align> with no value.",                    test_jhtml_hr_tag_003);
  CU_add_test(jhtml_suite, "test <hr align> with void value.",                  test_jhtml_hr_tag_004);
  CU_add_test(jhtml_suite, "test <hr align> with value(left).",                 test_jhtml_hr_tag_005);
  CU_add_test(jhtml_suite, "test <hr align> with value(right).",                test_jhtml_hr_tag_006);
  CU_add_test(jhtml_suite, "test <hr align> with value(center).",               test_jhtml_hr_tag_007);
  CU_add_test(jhtml_suite, "test <hr align> with value(unknown).",              test_jhtml_hr_tag_008);
  CU_add_test(jhtml_suite, "test <hr size> with no value.",                     test_jhtml_hr_tag_009);
  CU_add_test(jhtml_suite, "test <hr size> with void value.",                   test_jhtml_hr_tag_010);
  CU_add_test(jhtml_suite, "test <hr size> with non numeric value.",            test_jhtml_hr_tag_011);
  CU_add_test(jhtml_suite, "test <hr size> with numeric value.",                test_jhtml_hr_tag_012);
  CU_add_test(jhtml_suite, "test <hr width> with no value.",                    test_jhtml_hr_tag_013);
  CU_add_test(jhtml_suite, "test <hr width> with void value.",                  test_jhtml_hr_tag_014);
  CU_add_test(jhtml_suite, "test <hr width> with non numeric value.",           test_jhtml_hr_tag_015);
  CU_add_test(jhtml_suite, "test <hr width> with numeric value.",               test_jhtml_hr_tag_016);
  CU_add_test(jhtml_suite, "test <hr noshade>.",                                test_jhtml_hr_tag_017);
  CU_add_test(jhtml_suite, "test <hr color> 1.",                                test_jhtml_hr_tag_018);
  CU_add_test(jhtml_suite, "test <hr color> 2.",                                test_jhtml_hr_tag_018_1);
  CU_add_test(jhtml_suite, "test <hr color> 3.",                                test_jhtml_hr_tag_018_2);

  /*=========================================================================*/
  /* <HTML>                                                                  */
  /*=========================================================================*/
  CU_add_test(jhtml_suite, "test <html>.",                                      test_jhtml_html_tag_001);

  /*=========================================================================*/
  /* <IMG>                                                                   */
  /*=========================================================================*/
  CU_add_test(jhtml_suite, "test <img>." ,                                      test_jhtml_img_tag_001);
  CU_add_test(jhtml_suite, "test <img src> with no value." ,                    test_jhtml_img_tag_002);
  CU_add_test(jhtml_suite, "test <img src> with void value." ,                  test_jhtml_img_tag_003);
  CU_add_test(jhtml_suite, "test <img src> with alphabetic value." ,            test_jhtml_img_tag_004);
  CU_add_test(jhtml_suite, "test <img src> with japanese value." ,              test_jhtml_img_tag_005);
  CU_add_test(jhtml_suite, "test <img src> with japanese-hankaku value." ,      test_jhtml_img_tag_006);
  CU_add_test(jhtml_suite, "test <img align> with no value." ,                  test_jhtml_img_tag_007);
  CU_add_test(jhtml_suite, "test <img align> with void value." ,                test_jhtml_img_tag_008);
  CU_add_test(jhtml_suite, "test <img align> with value(top)." ,                test_jhtml_img_tag_009);
  CU_add_test(jhtml_suite, "test <img align> with value(middle)." ,             test_jhtml_img_tag_020);
  CU_add_test(jhtml_suite, "test <img align> with value(bottom)." ,             test_jhtml_img_tag_011);
  CU_add_test(jhtml_suite, "test <img align> with value(left)." ,               test_jhtml_img_tag_012);
  CU_add_test(jhtml_suite, "test <img align> with value(right)." ,              test_jhtml_img_tag_013);
  CU_add_test(jhtml_suite, "test <img align> with value(center)." ,             test_jhtml_img_tag_013_1);
  CU_add_test(jhtml_suite, "test <img align> with value(unkown)." ,             test_jhtml_img_tag_014);
  CU_add_test(jhtml_suite, "test <img width> with no value." ,                  test_jhtml_img_tag_015);
  CU_add_test(jhtml_suite, "test <img width> with void value." ,                test_jhtml_img_tag_016);
  CU_add_test(jhtml_suite, "test <img width> with alphabetic value." ,          test_jhtml_img_tag_017);
  CU_add_test(jhtml_suite, "test <img width> with numeric value." ,             test_jhtml_img_tag_018);
  CU_add_test(jhtml_suite, "test <img width> with percentage value." ,          test_jhtml_img_tag_019);
  CU_add_test(jhtml_suite, "test <img height> with no value." ,                 test_jhtml_img_tag_020);
  CU_add_test(jhtml_suite, "test <img height> with void value." ,               test_jhtml_img_tag_021);
  CU_add_test(jhtml_suite, "test <img height> with alphabetic value." ,         test_jhtml_img_tag_022);
  CU_add_test(jhtml_suite, "test <img height> with numeric value." ,            test_jhtml_img_tag_023);
  CU_add_test(jhtml_suite, "test <img height> with percentage value." ,         test_jhtml_img_tag_024);
  CU_add_test(jhtml_suite, "test <img hspace> with no value." ,                 test_jhtml_img_tag_025);
  CU_add_test(jhtml_suite, "test <img hspace> with void value." ,               test_jhtml_img_tag_026);
  CU_add_test(jhtml_suite, "test <img hspace> with alphabetic value." ,         test_jhtml_img_tag_027);
  CU_add_test(jhtml_suite, "test <img hspace> with numeric value." ,            test_jhtml_img_tag_028);
  CU_add_test(jhtml_suite, "test <img vspace> with no value." ,                 test_jhtml_img_tag_029);
  CU_add_test(jhtml_suite, "test <img vspace> with void value." ,               test_jhtml_img_tag_030);
  CU_add_test(jhtml_suite, "test <img vspace> with alphabetic value." ,         test_jhtml_img_tag_031);
  CU_add_test(jhtml_suite, "test <img vspace> with numeric value." ,            test_jhtml_img_tag_032);
  CU_add_test(jhtml_suite, "test <img alt>." ,                                  test_jhtml_img_tag_033);
  CU_add_test(jhtml_suite, "test <img alt> with void value." ,                  test_jhtml_img_tag_034);
  CU_add_test(jhtml_suite, "test <img alt> with alphabetic value." ,            test_jhtml_img_tag_035);
  CU_add_test(jhtml_suite, "test <img alt> with japanese value." ,              test_jhtml_img_tag_036);
  CU_add_test(jhtml_suite, "test <img alt> with japanese-hankaku value." ,      test_jhtml_img_tag_037);

  /*=========================================================================*/
  /* <INPUT>                                                                 */
  /*=========================================================================*/
  CU_add_test(jhtml_suite, "test <input>." ,                                    test_jhtml_input_tag_001);
  CU_add_test(jhtml_suite, "test <input type>." ,                               test_jhtml_input_tag_002);
  CU_add_test(jhtml_suite, "test <input type> with void value." ,               test_jhtml_input_tag_003);
  CU_add_test(jhtml_suite, "test <input type> with value(text)." ,              test_jhtml_input_tag_004);
  CU_add_test(jhtml_suite, "test <input type> with value(password)." ,          test_jhtml_input_tag_005);
  CU_add_test(jhtml_suite, "test <input type> with value(checkbox)." ,          test_jhtml_input_tag_006);
  CU_add_test(jhtml_suite, "test <input type> with value(radio)." ,             test_jhtml_input_tag_007);
  CU_add_test(jhtml_suite, "test <input type> with value(hidden)." ,            test_jhtml_input_tag_008);
  CU_add_test(jhtml_suite, "test <input type> with value(submit)." ,            test_jhtml_input_tag_009);
  CU_add_test(jhtml_suite, "test <input type> with value(reset)." ,             test_jhtml_input_tag_010);
  CU_add_test(jhtml_suite, "test <input type> with value(unknown)." ,           test_jhtml_input_tag_011);
  CU_add_test(jhtml_suite, "test <input name>." ,                               test_jhtml_input_tag_012);
  CU_add_test(jhtml_suite, "test <input name> with void value." ,               test_jhtml_input_tag_013);
  CU_add_test(jhtml_suite, "test <input name> with alphabetic value." ,         test_jhtml_input_tag_014);
  CU_add_test(jhtml_suite, "test <input name> with japanese value." ,           test_jhtml_input_tag_015);
  CU_add_test(jhtml_suite, "test <input name> with japanese-hankaku value." ,   test_jhtml_input_tag_016);
  CU_add_test(jhtml_suite, "test <input value>." ,                              test_jhtml_input_tag_017);
  CU_add_test(jhtml_suite, "test <input value> with void value." ,              test_jhtml_input_tag_018);
  CU_add_test(jhtml_suite, "test <input value> with alphabetic value." ,        test_jhtml_input_tag_019);
  CU_add_test(jhtml_suite, "test <input value> with japanese value." ,          test_jhtml_input_tag_020);
  CU_add_test(jhtml_suite, "test <input value> with japanese-hankaku value." ,  test_jhtml_input_tag_021);
  CU_add_test(jhtml_suite, "test <input size>." ,                               test_jhtml_input_tag_022);
  CU_add_test(jhtml_suite, "test <input size> with void value." ,               test_jhtml_input_tag_023);
  CU_add_test(jhtml_suite, "test <input size> with alphabetic value." ,         test_jhtml_input_tag_024);
  CU_add_test(jhtml_suite, "test <input size> with numeric value." ,            test_jhtml_input_tag_025);
  CU_add_test(jhtml_suite, "test <input maxlength>." ,                          test_jhtml_input_tag_026);
  CU_add_test(jhtml_suite, "test <input maxlength> with void value." ,          test_jhtml_input_tag_027);
  CU_add_test(jhtml_suite, "test <input maxlength> with alphabetic value." ,    test_jhtml_input_tag_028);
  CU_add_test(jhtml_suite, "test <input maxlength> with numeric value." ,       test_jhtml_input_tag_029);
  CU_add_test(jhtml_suite, "test <input checked>." ,                            test_jhtml_input_tag_030);
  CU_add_test(jhtml_suite, "test <input accesskey>." ,                          test_jhtml_input_tag_031);
  CU_add_test(jhtml_suite, "test <input accesskey> with void value." ,          test_jhtml_input_tag_032);
  CU_add_test(jhtml_suite, "test <input accesskey> with value." ,               test_jhtml_input_tag_033);
  CU_add_test(jhtml_suite, "test <input istyle> 1." ,                           test_jhtml_input_tag_034);
  CU_add_test(jhtml_suite, "test <input istyle> 2." ,                           test_jhtml_input_tag_035);
  CU_add_test(jhtml_suite, "test <input istyle> 3." ,                           test_jhtml_input_tag_036);
  CU_add_test(jhtml_suite, "test <input istyle> 4." ,                           test_jhtml_input_tag_037);
  CU_add_test(jhtml_suite, "test <input istyle> 5." ,                           test_jhtml_input_tag_038);
  CU_add_test(jhtml_suite, "test <input istyle> 6." ,                           test_jhtml_input_tag_039);
  CU_add_test(jhtml_suite, "test <input istyle> 7." ,                           test_jhtml_input_tag_040);
  CU_add_test(jhtml_suite, "test <input istyle> 8." ,                           test_jhtml_input_tag_041);
  CU_add_test(jhtml_suite, "test <input istyle> 9." ,                           test_jhtml_input_tag_042);
  CU_add_test(jhtml_suite, "test <input istyle> 10." ,                          test_jhtml_input_tag_043);
  CU_add_test(jhtml_suite, "test <input istyle> 11." ,                          test_jhtml_input_tag_044);
  CU_add_test(jhtml_suite, "test <input istyle> 12." ,                          test_jhtml_input_tag_045);
  CU_add_test(jhtml_suite, "test <input istyle> 13." ,                          test_jhtml_input_tag_046);
  CU_add_test(jhtml_suite, "test <input istyle> 14." ,                          test_jhtml_input_tag_047);

  /*=========================================================================*/
  /* <LI>                                                                    */
  /*=========================================================================*/
  CU_add_test(jhtml_suite, "test <li>." ,                                       test_jhtml_li_tag_001);
  CU_add_test(jhtml_suite, "test <li> with alphabetic value." ,                 test_jhtml_li_tag_002);
  CU_add_test(jhtml_suite, "test <li> with japanese value." ,                   test_jhtml_li_tag_003);
  CU_add_test(jhtml_suite, "test <li> with japanese-hankaku value." ,           test_jhtml_li_tag_004);
  CU_add_test(jhtml_suite, "test <li> type attribute 1." ,                      test_jhtml_li_tag_005);
  CU_add_test(jhtml_suite, "test <li> type attribute 2." ,                      test_jhtml_li_tag_006);
  CU_add_test(jhtml_suite, "test <li> type attribute 3." ,                      test_jhtml_li_tag_007);
  CU_add_test(jhtml_suite, "test <li> type attribute 4." ,                      test_jhtml_li_tag_008);
  CU_add_test(jhtml_suite, "test <li> type attribute 5." ,                      test_jhtml_li_tag_009);
  CU_add_test(jhtml_suite, "test <li> type attribute 6." ,                      test_jhtml_li_tag_010);
  CU_add_test(jhtml_suite, "test <li> type attribute 7." ,                      test_jhtml_li_tag_011);
  CU_add_test(jhtml_suite, "test <li> type attribute 8." ,                      test_jhtml_li_tag_012);
  CU_add_test(jhtml_suite, "test <li> type attribute 9." ,                      test_jhtml_li_tag_013);
  CU_add_test(jhtml_suite, "test <li> type attribute 10." ,                     test_jhtml_li_tag_014);
  CU_add_test(jhtml_suite, "test <li> type attribute 11." ,                     test_jhtml_li_tag_015);
  CU_add_test(jhtml_suite, "test <li> type attribute 12." ,                     test_jhtml_li_tag_016);
  /*=========================================================================*/
  /* <MENU>                                                                  */
  /*=========================================================================*/
  CU_add_test(jhtml_suite, "test <menu>." ,                                     test_jhtml_menu_tag_001);
  CU_add_test(jhtml_suite, "test <menu> 2." ,                                   test_jhtml_menu_tag_002);
  CU_add_test(jhtml_suite, "test <menu> 3." ,                                   test_jhtml_menu_tag_003);
  CU_add_test(jhtml_suite, "test <menu> 4." ,                                   test_jhtml_menu_tag_004);
  CU_add_test(jhtml_suite, "test <menu> 5." ,                                   test_jhtml_menu_tag_005);
  CU_add_test(jhtml_suite, "test <menu> 6." ,                                   test_jhtml_menu_tag_006);
  /*=========================================================================*/
  /* <OL>                                                                    */
  /*=========================================================================*/
  CU_add_test(jhtml_suite, "test <ol>." ,                                       test_jhtml_ol_tag_001);
  CU_add_test(jhtml_suite, "test <ol> 2." ,                                     test_jhtml_ol_tag_002);
  CU_add_test(jhtml_suite, "test <ol> 3." ,                                     test_jhtml_ol_tag_003);
  CU_add_test(jhtml_suite, "test <ol> 4." ,                                     test_jhtml_ol_tag_004);
  CU_add_test(jhtml_suite, "test <ol> 5." ,                                     test_jhtml_ol_tag_005);
  CU_add_test(jhtml_suite, "test <ol> 6." ,                                     test_jhtml_ol_tag_006);
  CU_add_test(jhtml_suite, "test <ol> 7." ,                                     test_jhtml_ol_tag_007);
  CU_add_test(jhtml_suite, "test <ol> 8." ,                                     test_jhtml_ol_tag_008);
  CU_add_test(jhtml_suite, "test <ol> 9." ,                                     test_jhtml_ol_tag_009);
  CU_add_test(jhtml_suite, "test <ol> 10." ,                                    test_jhtml_ol_tag_010);
  CU_add_test(jhtml_suite, "test <ol> 11." ,                                    test_jhtml_ol_tag_011);
  CU_add_test(jhtml_suite, "test <ol> 12." ,                                    test_jhtml_ol_tag_012);
  CU_add_test(jhtml_suite, "test <ol> 13." ,                                    test_jhtml_ol_tag_013);
  CU_add_test(jhtml_suite, "test <ol> 14." ,                                    test_jhtml_ol_tag_014);
  CU_add_test(jhtml_suite, "test <ol> 15." ,                                    test_jhtml_ol_tag_015);
  CU_add_test(jhtml_suite, "test <ol> 16." ,                                    test_jhtml_ol_tag_016);
  CU_add_test(jhtml_suite, "test <ol> 17." ,                                    test_jhtml_ol_tag_017);
  CU_add_test(jhtml_suite, "test <ol> 18." ,                                    test_jhtml_ol_tag_018);
  /*=========================================================================*/
  /* <OPTION>                                                                */
  /*=========================================================================*/
  CU_add_test(jhtml_suite, "test <option>." ,                                   test_jhtml_option_tag_001);
  CU_add_test(jhtml_suite, "test <option value> with no value." ,               test_jhtml_option_tag_002);
  CU_add_test(jhtml_suite, "test <option value> with void value." ,             test_jhtml_option_tag_003);
  CU_add_test(jhtml_suite, "test <option value> with alphabetic value." ,       test_jhtml_option_tag_004);
  CU_add_test(jhtml_suite, "test <option value> with japanese value." ,         test_jhtml_option_tag_005);
  CU_add_test(jhtml_suite, "test <option value> with japanese-kana value." ,    test_jhtml_option_tag_006);
  CU_add_test(jhtml_suite, "test <option selected>." ,                          test_jhtml_option_tag_007);

  /*=========================================================================*/
  /* <P>                                                                     */
  /*=========================================================================*/
  CU_add_test(jhtml_suite, "test <p> 1." ,                                      test_jhtml_p_tag_001);
  CU_add_test(jhtml_suite, "test <p> 2." ,                                      test_jhtml_p_tag_002);
  CU_add_test(jhtml_suite, "test <p> 3." ,                                      test_jhtml_p_tag_003);
  CU_add_test(jhtml_suite, "test <p> 4." ,                                      test_jhtml_p_tag_004);
  CU_add_test(jhtml_suite, "test <p> 5." ,                                      test_jhtml_p_tag_005);
  CU_add_test(jhtml_suite, "test <p> 6." ,                                      test_jhtml_p_tag_006);
  CU_add_test(jhtml_suite, "test <p> 7." ,                                      test_jhtml_p_tag_007);
  CU_add_test(jhtml_suite, "test <p> 8." ,                                      test_jhtml_p_tag_008);

  /*=========================================================================*/
  /* <PLAINTEXT>                                                             */
  /*=========================================================================*/
  CU_add_test(jhtml_suite, "test <plaintext> 1." ,                              test_jhtml_plaintext_tag_001);
  CU_add_test(jhtml_suite, "test <plaintext> 2." ,                              test_jhtml_plaintext_tag_002);
  CU_add_test(jhtml_suite, "test <plaintext> 3. value is not closed." ,         test_jhtml_plaintext_tag_003);
  CU_add_test(jhtml_suite, "test <plaintext> 3. value is not closed and hankaku." , test_jhtml_plaintext_tag_004);

  /*=========================================================================*/
  /* <PRE>                                                                   */
  /*=========================================================================*/
  CU_add_test(jhtml_suite, "test <pre> 1." ,                                    test_jhtml_pre_tag_001);
  CU_add_test(jhtml_suite, "test <pre> 2." ,                                    test_jhtml_pre_tag_002);
  CU_add_test(jhtml_suite, "test <pre> 3." ,                                    test_jhtml_pre_tag_003);
  CU_add_test(jhtml_suite, "test <pre> 4." ,                                    test_jhtml_pre_tag_004);
  CU_add_test(jhtml_suite, "test <pre> 5." ,                                    test_jhtml_pre_tag_005);

  /*=========================================================================*/
  /* <SELECT>                                                                */
  /*=========================================================================*/
  CU_add_test(jhtml_suite, "test <select> 1." ,                                 test_jhtml_select_tag_001);
  CU_add_test(jhtml_suite, "test <select> 2." ,                                 test_jhtml_select_tag_002);
  CU_add_test(jhtml_suite, "test <select> 3." ,                                 test_jhtml_select_tag_003);
  CU_add_test(jhtml_suite, "test <select> 4." ,                                 test_jhtml_select_tag_004);
  CU_add_test(jhtml_suite, "test <select> 5." ,                                 test_jhtml_select_tag_005);
  CU_add_test(jhtml_suite, "test <select> 6." ,                                 test_jhtml_select_tag_006);
  CU_add_test(jhtml_suite, "test <select> 7." ,                                 test_jhtml_select_tag_007);
  CU_add_test(jhtml_suite, "test <select> 8." ,                                 test_jhtml_select_tag_008);
  CU_add_test(jhtml_suite, "test <select> 9." ,                                 test_jhtml_select_tag_009);
  CU_add_test(jhtml_suite, "test <select> 10." ,                                test_jhtml_select_tag_010);
  CU_add_test(jhtml_suite, "test <select> 11." ,                                test_jhtml_select_tag_011);
  CU_add_test(jhtml_suite, "test <select> 12." ,                                test_jhtml_select_tag_012);
  CU_add_test(jhtml_suite, "test <select> 13." ,                                test_jhtml_select_tag_013);
  CU_add_test(jhtml_suite, "test <select> 14." ,                                test_jhtml_select_tag_014);

  /*=========================================================================*/
  /* <TEXTAREA>                                                              */
  /*=========================================================================*/
  CU_add_test(jhtml_suite, "test <textarea> 1." ,                               test_jhtml_textarea_tag_001);
  CU_add_test(jhtml_suite, "test <textarea> 2." ,                               test_jhtml_textarea_tag_002);
  CU_add_test(jhtml_suite, "test <textarea> 3." ,                               test_jhtml_textarea_tag_003);
  CU_add_test(jhtml_suite, "test <textarea> 4." ,                               test_jhtml_textarea_tag_004);
  CU_add_test(jhtml_suite, "test <textarea> 5." ,                               test_jhtml_textarea_tag_005);
  CU_add_test(jhtml_suite, "test <textarea> 6." ,                               test_jhtml_textarea_tag_006);
  CU_add_test(jhtml_suite, "test <textarea> 7." ,                               test_jhtml_textarea_tag_007);
  CU_add_test(jhtml_suite, "test <textarea> 8." ,                               test_jhtml_textarea_tag_008);
  CU_add_test(jhtml_suite, "test <textarea> 9." ,                               test_jhtml_textarea_tag_009);
  CU_add_test(jhtml_suite, "test <textarea> 10." ,                              test_jhtml_textarea_tag_010);
  CU_add_test(jhtml_suite, "test <textarea> 11." ,                              test_jhtml_textarea_tag_011);
  CU_add_test(jhtml_suite, "test <textarea> 12." ,                              test_jhtml_textarea_tag_012);
  CU_add_test(jhtml_suite, "test <textarea> 13." ,                              test_jhtml_textarea_tag_013);
  CU_add_test(jhtml_suite, "test <textarea> 14." ,                              test_jhtml_textarea_tag_014);
  CU_add_test(jhtml_suite, "test <textarea> 15." ,                              test_jhtml_textarea_tag_015);
  CU_add_test(jhtml_suite, "test <textarea> 16." ,                              test_jhtml_textarea_tag_016);
  CU_add_test(jhtml_suite, "test <textarea> 17." ,                              test_jhtml_textarea_tag_017);
  CU_add_test(jhtml_suite, "test <textarea> 18." ,                              test_jhtml_textarea_tag_018);
  CU_add_test(jhtml_suite, "test <textarea> 19." ,                              test_jhtml_textarea_tag_019);
  CU_add_test(jhtml_suite, "test <textarea> 20." ,                              test_jhtml_textarea_tag_020);
  CU_add_test(jhtml_suite, "test <textarea> 21." ,                              test_jhtml_textarea_tag_021);
  CU_add_test(jhtml_suite, "test <textarea> 22." ,                              test_jhtml_textarea_tag_022);
  CU_add_test(jhtml_suite, "test <textarea> 23." ,                              test_jhtml_textarea_tag_023);
  CU_add_test(jhtml_suite, "test <textarea> 24." ,                              test_jhtml_textarea_tag_024);
  CU_add_test(jhtml_suite, "test <textarea> 25." ,                              test_jhtml_textarea_tag_025);
  CU_add_test(jhtml_suite, "test <textarea> 26." ,                              test_jhtml_textarea_tag_026);

  /*=========================================================================*/
  /* <TITLE>                                                                 */
  /*=========================================================================*/
  CU_add_test(jhtml_suite, "test <title> 1." ,                                  test_jhtml_title_tag_001);
  CU_add_test(jhtml_suite, "test <title> 2." ,                                  test_jhtml_title_tag_002);

  /*=========================================================================*/
  /* <UL>                                                                    */
  /*=========================================================================*/
  CU_add_test(jhtml_suite, "test <ul> 1." ,                                     test_jhtml_ul_tag_001);
  CU_add_test(jhtml_suite, "test <ul> 2." ,                                     test_jhtml_ul_tag_002);
  CU_add_test(jhtml_suite, "test <ul> 3." ,                                     test_jhtml_ul_tag_003);
  CU_add_test(jhtml_suite, "test <ul> 4." ,                                     test_jhtml_ul_tag_004);
  CU_add_test(jhtml_suite, "test <ul> 5." ,                                     test_jhtml_ul_tag_005);
  CU_add_test(jhtml_suite, "test <ul> 6." ,                                     test_jhtml_ul_tag_006);

  /*=========================================================================*/
  /* <BLINK>                                                                 */
  /*=========================================================================*/
  CU_add_test(jhtml_suite, "test <blink> 1." ,                                  test_jhtml_blink_tag_001);
  CU_add_test(jhtml_suite, "test <blink> 2." ,                                  test_jhtml_blink_tag_002);
  CU_add_test(jhtml_suite, "test <blink> 3." ,                                  test_jhtml_blink_tag_003);
  CU_add_test(jhtml_suite, "test <blink> 4." ,                                  test_jhtml_blink_tag_004);

  /*=========================================================================*/
  /* <MARQUEE>                                                               */
  /*=========================================================================*/
  CU_add_test(jhtml_suite, "test <marquee> 1." ,                                test_jhtml_marquee_tag_001);
  CU_add_test(jhtml_suite, "test <marquee> 2." ,                                test_jhtml_marquee_tag_002);
  CU_add_test(jhtml_suite, "test <marquee> 3." ,                                test_jhtml_marquee_tag_003);
  CU_add_test(jhtml_suite, "test <marquee> 4." ,                                test_jhtml_marquee_tag_004);
  CU_add_test(jhtml_suite, "test <marquee> 5." ,                                test_jhtml_marquee_tag_005);
  CU_add_test(jhtml_suite, "test <marquee> 6." ,                                test_jhtml_marquee_tag_006);
  CU_add_test(jhtml_suite, "test <marquee> 7." ,                                test_jhtml_marquee_tag_007);
  CU_add_test(jhtml_suite, "test <marquee> 8." ,                                test_jhtml_marquee_tag_008);
  CU_add_test(jhtml_suite, "test <marquee> 9." ,                                test_jhtml_marquee_tag_009);
  CU_add_test(jhtml_suite, "test <marquee> 10." ,                               test_jhtml_marquee_tag_010);
  CU_add_test(jhtml_suite, "test <marquee> 11." ,                               test_jhtml_marquee_tag_011);
  CU_add_test(jhtml_suite, "test <marquee> 12." ,                               test_jhtml_marquee_tag_012);
  CU_add_test(jhtml_suite, "test <marquee> 13." ,                               test_jhtml_marquee_tag_013);
  CU_add_test(jhtml_suite, "test <marquee> 14." ,                               test_jhtml_marquee_tag_014);
  CU_add_test(jhtml_suite, "test <marquee> 15." ,                               test_jhtml_marquee_tag_015);
  CU_add_test(jhtml_suite, "test <marquee> 16." ,                               test_jhtml_marquee_tag_016);
  CU_add_test(jhtml_suite, "test <marquee> 17." ,                               test_jhtml_marquee_tag_017);
  CU_add_test(jhtml_suite, "test <marquee> 18." ,                               test_jhtml_marquee_tag_018);
  CU_add_test(jhtml_suite, "test <marquee> 19." ,                               test_jhtml_marquee_tag_019);
  CU_add_test(jhtml_suite, "test <marquee> 20." ,                               test_jhtml_marquee_tag_020);
  CU_add_test(jhtml_suite, "test <marquee> 21." ,                               test_jhtml_marquee_tag_021);

  /*=========================================================================*/
  /* <META>                                                                  */
  /*=========================================================================*/
  CU_add_test(jhtml_suite, "test <meta> 1." ,                                   test_jhtml_meta_tag_001);
  CU_add_test(jhtml_suite, "test <meta> 2." ,                                   test_jhtml_meta_tag_002);
  CU_add_test(jhtml_suite, "test <meta> 3." ,                                   test_jhtml_meta_tag_003);
  CU_add_test(jhtml_suite, "test <meta> 4." ,                                   test_jhtml_meta_tag_004);
  CU_add_test(jhtml_suite, "test <meta> 5." ,                                   test_jhtml_meta_tag_005);
  CU_add_test(jhtml_suite, "test <meta> 6." ,                                   test_jhtml_meta_tag_006);
  CU_add_test(jhtml_suite, "test <meta> 7." ,                                   test_jhtml_meta_tag_007);
  CU_add_test(jhtml_suite, "test <meta> 8." ,                                   test_jhtml_meta_tag_008);
  CU_add_test(jhtml_suite, "test <meta> 9." ,                                   test_jhtml_meta_tag_009);

  /*=========================================================================*/
  /* <font>                                                                  */
  /*=========================================================================*/
  CU_add_test(jhtml_suite, "test <font> 1." ,                                   test_jhtml_font_tag_001);
  CU_add_test(jhtml_suite, "test <font> 2." ,                                   test_jhtml_font_tag_002);
  CU_add_test(jhtml_suite, "test <font> 3." ,                                   test_jhtml_font_tag_003);
  CU_add_test(jhtml_suite, "test <font> 4." ,                                   test_jhtml_font_tag_004);
  CU_add_test(jhtml_suite, "test <font> 5." ,                                   test_jhtml_font_tag_005);
  CU_add_test(jhtml_suite, "test <font> 6." ,                                   test_jhtml_font_tag_006);
  CU_add_test(jhtml_suite, "test <font> 7." ,                                   test_jhtml_font_tag_007);
  CU_add_test(jhtml_suite, "test <font> 8." ,                                   test_jhtml_font_tag_008);
  CU_add_test(jhtml_suite, "test <font> 9." ,                                   test_jhtml_font_tag_009);
  CU_add_test(jhtml_suite, "test <font> 10." ,                                  test_jhtml_font_tag_010);
  /*=========================================================================*/
  /* <param>                                                                 */
  /*=========================================================================*/
  CU_add_test(jhtml_suite, "test <param> 1." ,                                  test_jhtml_param_tag_001);

  /* CSS */
  CU_add_test(jhtml_suite, "test link 001." ,                                    test_jhtml_link_001);
  CU_add_test(jhtml_suite, "test link 002." ,                                    test_jhtml_link_002);
  CU_add_test(jhtml_suite, "test link 003." ,                                    test_jhtml_link_003);
  CU_add_test(jhtml_suite, "test link 004." ,                                    test_jhtml_link_004);
  CU_add_test(jhtml_suite, "test link 005." ,                                    test_jhtml_link_005);
  CU_add_test(jhtml_suite, "test link 006." ,                                    test_jhtml_link_006);
  CU_add_test(jhtml_suite, "test link 007." ,                                    test_jhtml_link_007);
  CU_add_test(jhtml_suite, "test link 008." ,                                    test_jhtml_link_008);
  CU_add_test(jhtml_suite, "test link 009." ,                                    test_jhtml_link_009);

  CU_add_test(jhtml_suite, "test html with css 001",                             test_jhtml_html_tag_with_css_001);
  CU_add_test(jhtml_suite, "test meta with css 001",                             test_jhtml_meta_tag_with_css_001);

  CU_add_test(jhtml_suite, "test textarea with css 001",                         test_jhtml_textarea_tag_with_css_001);
  CU_add_test(jhtml_suite, "test textarea with css 002",                         test_jhtml_textarea_tag_with_css_002);
  CU_add_test(jhtml_suite, "test textarea with css 003",                         test_jhtml_textarea_tag_with_css_003);
  CU_add_test(jhtml_suite, "test textarea with css 004",                         test_jhtml_textarea_tag_with_css_004);

  CU_add_test(jhtml_suite, "test textarea with css 005",                         test_jhtml_textarea_tag_with_css_005);
  CU_add_test(jhtml_suite, "test textarea with css 006",                         test_jhtml_textarea_tag_with_css_006);
  CU_add_test(jhtml_suite, "test textarea with css 007",                         test_jhtml_textarea_tag_with_css_007);
  CU_add_test(jhtml_suite, "test textarea with css 008",                         test_jhtml_textarea_tag_with_css_008);

  CU_add_test(jhtml_suite, "test p with css 001",                                test_jhtml_p_tag_with_css_001);
  CU_add_test(jhtml_suite, "test p with css 002",                                test_jhtml_p_tag_with_css_002);
  CU_add_test(jhtml_suite, "test p with css 003",                                test_jhtml_p_tag_with_css_003);
  CU_add_test(jhtml_suite, "test p with css 004",                                test_jhtml_p_tag_with_css_004);
  CU_add_test(jhtml_suite, "test p with css 005",                                test_jhtml_p_tag_with_css_005);
  CU_add_test(jhtml_suite, "test p with css 006",                                test_jhtml_p_tag_with_css_006);
  CU_add_test(jhtml_suite, "test p with css 007",                                test_jhtml_p_tag_with_css_007);
  CU_add_test(jhtml_suite, "test p with css 008",                                test_jhtml_p_tag_with_css_008);
  CU_add_test(jhtml_suite, "test p with css 010",                                test_jhtml_p_tag_with_css_010);
  CU_add_test(jhtml_suite, "test p with css 011",                                test_jhtml_p_tag_with_css_011);
  CU_add_test(jhtml_suite, "test p with css 012",                                test_jhtml_p_tag_with_css_012);
  CU_add_test(jhtml_suite, "test p with css 013",                                test_jhtml_p_tag_with_css_013);

  CU_add_test(jhtml_suite, "test li with css 001",                               test_jhtml_li_tag_with_css_001);
  CU_add_test(jhtml_suite, "test li with css 002",                               test_jhtml_li_tag_with_css_002);
  CU_add_test(jhtml_suite, "test li with css 003",                               test_jhtml_li_tag_with_css_003);
  CU_add_test(jhtml_suite, "test li with css 004",                               test_jhtml_li_tag_with_css_004);
  CU_add_test(jhtml_suite, "test li with css 005",                               test_jhtml_li_tag_with_css_005);
  CU_add_test(jhtml_suite, "test li with css 006",                               test_jhtml_li_tag_with_css_006);
  CU_add_test(jhtml_suite, "test li with css 007",                               test_jhtml_li_tag_with_css_007);
  CU_add_test(jhtml_suite, "test li with css 008",                               test_jhtml_li_tag_with_css_008);
  CU_add_test(jhtml_suite, "test li with css 009",                               test_jhtml_li_tag_with_css_009);
  CU_add_test(jhtml_suite, "test li with css 010",                               test_jhtml_li_tag_with_css_010);
  CU_add_test(jhtml_suite, "test li with css 011",                               test_jhtml_li_tag_with_css_011);
  CU_add_test(jhtml_suite, "test li with css 012",                               test_jhtml_li_tag_with_css_012);

  CU_add_test(jhtml_suite, "test ol with css 001",                               test_jhtml_ol_tag_with_css_001);
  CU_add_test(jhtml_suite, "test ol with css 002",                               test_jhtml_ol_tag_with_css_002);
  CU_add_test(jhtml_suite, "test ol with css 003",                               test_jhtml_ol_tag_with_css_003);
  CU_add_test(jhtml_suite, "test ol with css 004",                               test_jhtml_ol_tag_with_css_004);
  CU_add_test(jhtml_suite, "test ol with css 005",                               test_jhtml_ol_tag_with_css_005);
  CU_add_test(jhtml_suite, "test ol with css 006",                               test_jhtml_ol_tag_with_css_006);

  CU_add_test(jhtml_suite, "test h1 with css 001",                               test_jhtml_h1_tag_with_css_001);
  CU_add_test(jhtml_suite, "test h1 with css 002",                               test_jhtml_h1_tag_with_css_002);
  CU_add_test(jhtml_suite, "test h1 with css 003",                               test_jhtml_h1_tag_with_css_003);
  CU_add_test(jhtml_suite, "test h1 with css 004",                               test_jhtml_h1_tag_with_css_004);
  CU_add_test(jhtml_suite, "test h1 with css 005",                               test_jhtml_h1_tag_with_css_005);
  CU_add_test(jhtml_suite, "test h1 with css 006",                               test_jhtml_h1_tag_with_css_006);

  CU_add_test(jhtml_suite, "test h2 with css 001",                               test_jhtml_h2_tag_with_css_001);
  CU_add_test(jhtml_suite, "test h2 with css 002",                               test_jhtml_h2_tag_with_css_002);
  CU_add_test(jhtml_suite, "test h2 with css 003",                               test_jhtml_h2_tag_with_css_003);
  CU_add_test(jhtml_suite, "test h2 with css 004",                               test_jhtml_h2_tag_with_css_004);
  CU_add_test(jhtml_suite, "test h2 with css 005",                               test_jhtml_h2_tag_with_css_005);
  CU_add_test(jhtml_suite, "test h2 with css 006",                               test_jhtml_h2_tag_with_css_006);

  CU_add_test(jhtml_suite, "test h3 with css 001",                               test_jhtml_h3_tag_with_css_001);
  CU_add_test(jhtml_suite, "test h3 with css 002",                               test_jhtml_h3_tag_with_css_002);
  CU_add_test(jhtml_suite, "test h3 with css 003",                               test_jhtml_h3_tag_with_css_003);
  CU_add_test(jhtml_suite, "test h3 with css 004",                               test_jhtml_h3_tag_with_css_004);
  CU_add_test(jhtml_suite, "test h3 with css 005",                               test_jhtml_h3_tag_with_css_005);
  CU_add_test(jhtml_suite, "test h3 with css 006",                               test_jhtml_h3_tag_with_css_006);

  CU_add_test(jhtml_suite, "test h4 with css 001",                               test_jhtml_h4_tag_with_css_001);
  CU_add_test(jhtml_suite, "test h4 with css 002",                               test_jhtml_h4_tag_with_css_002);
  CU_add_test(jhtml_suite, "test h4 with css 003",                               test_jhtml_h4_tag_with_css_003);
  CU_add_test(jhtml_suite, "test h4 with css 004",                               test_jhtml_h4_tag_with_css_004);
  CU_add_test(jhtml_suite, "test h4 with css 005",                               test_jhtml_h4_tag_with_css_005);
  CU_add_test(jhtml_suite, "test h4 with css 006",                               test_jhtml_h4_tag_with_css_006);

  CU_add_test(jhtml_suite, "test h5 with css 001",                               test_jhtml_h5_tag_with_css_001);
  CU_add_test(jhtml_suite, "test h5 with css 002",                               test_jhtml_h5_tag_with_css_002);
  CU_add_test(jhtml_suite, "test h5 with css 003",                               test_jhtml_h5_tag_with_css_003);
  CU_add_test(jhtml_suite, "test h5 with css 004",                               test_jhtml_h5_tag_with_css_004);
  CU_add_test(jhtml_suite, "test h5 with css 005",                               test_jhtml_h5_tag_with_css_005);
  CU_add_test(jhtml_suite, "test h5 with css 006",                               test_jhtml_h5_tag_with_css_006);

  CU_add_test(jhtml_suite, "test h6 with css 001",                               test_jhtml_h6_tag_with_css_001);
  CU_add_test(jhtml_suite, "test h6 with css 002",                               test_jhtml_h6_tag_with_css_002);
  CU_add_test(jhtml_suite, "test h6 with css 003",                               test_jhtml_h6_tag_with_css_003);
  CU_add_test(jhtml_suite, "test h6 with css 004",                               test_jhtml_h6_tag_with_css_004);
  CU_add_test(jhtml_suite, "test h6 with css 005",                               test_jhtml_h6_tag_with_css_005);
  CU_add_test(jhtml_suite, "test h6 with css 006",                               test_jhtml_h6_tag_with_css_006);

  CU_add_test(jhtml_suite, "test body with css 001",                             test_jhtml_body_tag_with_css_001);
  CU_add_test(jhtml_suite, "test body with css 002",                             test_jhtml_body_tag_with_css_002);
  CU_add_test(jhtml_suite, "test body with css 003",                             test_jhtml_body_tag_with_css_003);
  CU_add_test(jhtml_suite, "test body with css 004",                             test_jhtml_body_tag_with_css_004);
  CU_add_test(jhtml_suite, "test body with css 005",                             test_jhtml_body_tag_with_css_005);
  CU_add_test(jhtml_suite, "test body with css 006",                             test_jhtml_body_tag_with_css_006);

  CU_add_test(jhtml_suite, "test font with css 001",                             test_jhtml_font_tag_with_css_001);
  CU_add_test(jhtml_suite, "test font with css 002",                             test_jhtml_font_tag_with_css_002);

  CU_add_test(jhtml_suite, "test form with css 001",                             test_jhtml_form_tag_with_css_001);
  CU_add_test(jhtml_suite, "test form with css 002",                             test_jhtml_form_tag_with_css_002);
  CU_add_test(jhtml_suite, "test form with css 003",                             test_jhtml_form_tag_with_css_003);
  CU_add_test(jhtml_suite, "test form with css 004",                             test_jhtml_form_tag_with_css_004);
  CU_add_test(jhtml_suite, "test form with css 005",                             test_jhtml_form_tag_with_css_005);
  CU_add_test(jhtml_suite, "test form with css 006",                             test_jhtml_form_tag_with_css_006);
  CU_add_test(jhtml_suite, "test form with css 007",                             test_jhtml_form_tag_with_css_007);
  CU_add_test(jhtml_suite, "test form with css 008",                             test_jhtml_form_tag_with_css_008);

  CU_add_test(jhtml_suite, "test input with css 001",                            test_jhtml_input_tag_with_css_001);
  CU_add_test(jhtml_suite, "test input with css 002",                            test_jhtml_input_tag_with_css_002);
  CU_add_test(jhtml_suite, "test input with css 003",                            test_jhtml_input_tag_with_css_003);
  CU_add_test(jhtml_suite, "test input with css 004",                            test_jhtml_input_tag_with_css_004);
  CU_add_test(jhtml_suite, "test input with css 005",                            test_jhtml_input_tag_with_css_005);
  CU_add_test(jhtml_suite, "test input with css 006",                            test_jhtml_input_tag_with_css_006);
  CU_add_test(jhtml_suite, "test input with css 007",                            test_jhtml_input_tag_with_css_007);
  CU_add_test(jhtml_suite, "test input with css 008",                            test_jhtml_input_tag_with_css_008);

  CU_add_test(jhtml_suite, "test center with css 001",                           test_jhtml_center_tag_with_css_001);
  CU_add_test(jhtml_suite, "test center with css 002",                           test_jhtml_center_tag_with_css_002);
  CU_add_test(jhtml_suite, "test center with css 003",                           test_jhtml_center_tag_with_css_003);
  CU_add_test(jhtml_suite, "test center with css 004",                           test_jhtml_center_tag_with_css_004);

  CU_add_test(jhtml_suite, "test hr with css 001",                               test_jhtml_hr_tag_with_css_001);
  CU_add_test(jhtml_suite, "test hr with css 002",                               test_jhtml_hr_tag_with_css_002);
  CU_add_test(jhtml_suite, "test hr with css 003",                               test_jhtml_hr_tag_with_css_003);
  CU_add_test(jhtml_suite, "test hr with css 004",                               test_jhtml_hr_tag_with_css_004);
  CU_add_test(jhtml_suite, "test hr with css 005",                               test_jhtml_hr_tag_with_css_005);
  CU_add_test(jhtml_suite, "test hr with css 006",                               test_jhtml_hr_tag_with_css_006);
  CU_add_test(jhtml_suite, "test hr with css 007",                               test_jhtml_hr_tag_with_css_007);
  CU_add_test(jhtml_suite, "test hr with css 008",                               test_jhtml_hr_tag_with_css_008);

  CU_add_test(jhtml_suite, "test img with css 001",                              test_jhtml_img_tag_with_css_001);
  CU_add_test(jhtml_suite, "test img with css 002",                              test_jhtml_img_tag_with_css_002);
  CU_add_test(jhtml_suite, "test img with css 003",                              test_jhtml_img_tag_with_css_003);
  CU_add_test(jhtml_suite, "test img with css 004",                              test_jhtml_img_tag_with_css_004);
  CU_add_test(jhtml_suite, "test img with css 005",                              test_jhtml_img_tag_with_css_005);
  CU_add_test(jhtml_suite, "test img with css 006",                              test_jhtml_img_tag_with_css_006);
  CU_add_test(jhtml_suite, "test img with css 007",                              test_jhtml_img_tag_with_css_007);
  CU_add_test(jhtml_suite, "test img with css 008",                              test_jhtml_img_tag_with_css_008);
  CU_add_test(jhtml_suite, "test img with css 009",                              test_jhtml_img_tag_with_css_009);
  CU_add_test(jhtml_suite, "test img with css 010",                              test_jhtml_img_tag_with_css_010);

  CU_add_test(jhtml_suite, "test div with css 001",                              test_jhtml_div_tag_with_css_001);
  CU_add_test(jhtml_suite, "test div with css 002",                              test_jhtml_div_tag_with_css_002);
  CU_add_test(jhtml_suite, "test div with css 003",                              test_jhtml_div_tag_with_css_003);
  CU_add_test(jhtml_suite, "test div with css 004",                              test_jhtml_div_tag_with_css_004);
  CU_add_test(jhtml_suite, "test div with css 005",                              test_jhtml_div_tag_with_css_005);
  CU_add_test(jhtml_suite, "test div with css 006",                              test_jhtml_div_tag_with_css_006);
  CU_add_test(jhtml_suite, "test div with css 007",                              test_jhtml_div_tag_with_css_007);
  CU_add_test(jhtml_suite, "test div with css 008",                              test_jhtml_div_tag_with_css_008);
  CU_add_test(jhtml_suite, "test div with css 009",                              test_jhtml_div_tag_with_css_009);
  CU_add_test(jhtml_suite, "test div with css 010",                              test_jhtml_div_tag_with_css_010);
  CU_add_test(jhtml_suite, "test div with css 011",                              test_jhtml_div_tag_with_css_011);
  CU_add_test(jhtml_suite, "test div with css 012",                              test_jhtml_div_tag_with_css_012);
  CU_add_test(jhtml_suite, "test div with css 013",                              test_jhtml_div_tag_with_css_013);
  CU_add_test(jhtml_suite, "test div with css 014",                              test_jhtml_div_tag_with_css_014);
  CU_add_test(jhtml_suite, "test div with css 015",                              test_jhtml_div_tag_with_css_015);
  CU_add_test(jhtml_suite, "test div with css 016",                              test_jhtml_div_tag_with_css_016);
  CU_add_test(jhtml_suite, "test div with css 017",                              test_jhtml_div_tag_with_css_017);
  CU_add_test(jhtml_suite, "test div with css 018",                              test_jhtml_div_tag_with_css_018);
  CU_add_test(jhtml_suite, "test div with css 019",                              test_jhtml_div_tag_with_css_019);
  CU_add_test(jhtml_suite, "test div with css 020",                              test_jhtml_div_tag_with_css_020);
  CU_add_test(jhtml_suite, "test div with css 021",                              test_jhtml_div_tag_with_css_021);
  CU_add_test(jhtml_suite, "test div with css 022",                              test_jhtml_div_tag_with_css_022);
  CU_add_test(jhtml_suite, "test div with css 023",                              test_jhtml_div_tag_with_css_023);
  CU_add_test(jhtml_suite, "test div with css 024",                              test_jhtml_div_tag_with_css_024);
  CU_add_test(jhtml_suite, "test div with css 025",                              test_jhtml_div_tag_with_css_025);
  CU_add_test(jhtml_suite, "test div with css 026",                              test_jhtml_div_tag_with_css_026);
  CU_add_test(jhtml_suite, "test div with css 027",                              test_jhtml_div_tag_with_css_027);
  CU_add_test(jhtml_suite, "test div with css 028",                              test_jhtml_div_tag_with_css_028);
  CU_add_test(jhtml_suite, "test div with css 029",                              test_jhtml_div_tag_with_css_029);
  CU_add_test(jhtml_suite, "test div with css 030",                              test_jhtml_div_tag_with_css_030);
  CU_add_test(jhtml_suite, "test div with css 031",                              test_jhtml_div_tag_with_css_031);
  CU_add_test(jhtml_suite, "test div with css 032",                              test_jhtml_div_tag_with_css_032);
  CU_add_test(jhtml_suite, "test div with css 033",                              test_jhtml_div_tag_with_css_033);
  CU_add_test(jhtml_suite, "test div with css 034",                              test_jhtml_div_tag_with_css_034);
  CU_add_test(jhtml_suite, "test div with css 035",                              test_jhtml_div_tag_with_css_035);
  CU_add_test(jhtml_suite, "test div with css 036",                              test_jhtml_div_tag_with_css_036);
  CU_add_test(jhtml_suite, "test div with css 037",                              test_jhtml_div_tag_with_css_037);
  CU_add_test(jhtml_suite, "test div with css 038",                              test_jhtml_div_tag_with_css_038);
  CU_add_test(jhtml_suite, "test div with css 039",                              test_jhtml_div_tag_with_css_039);
  CU_add_test(jhtml_suite, "test div with css 040",                              test_jhtml_div_tag_with_css_040);
  CU_add_test(jhtml_suite, "test div with css 041",                              test_jhtml_div_tag_with_css_041);
  CU_add_test(jhtml_suite, "test div with css 042",                              test_jhtml_div_tag_with_css_042);

  CU_add_test(jhtml_suite, "test dt with css 001",                              test_jhtml_dt_tag_with_css_001);
  CU_add_test(jhtml_suite, "test dt with css 002",                              test_jhtml_dt_tag_with_css_002);
  CU_add_test(jhtml_suite, "test dt with css 003",                              test_jhtml_dt_tag_with_css_003);
  CU_add_test(jhtml_suite, "test dt with css 004",                              test_jhtml_dt_tag_with_css_004);
  CU_add_test(jhtml_suite, "test dt with css 005",                              test_jhtml_dt_tag_with_css_005);
  CU_add_test(jhtml_suite, "test dt with css 006",                              test_jhtml_dt_tag_with_css_006);
  CU_add_test(jhtml_suite, "test dt with css 007",                              test_jhtml_dt_tag_with_css_007);
  CU_add_test(jhtml_suite, "test dt with css 008",                              test_jhtml_dt_tag_with_css_008);
  CU_add_test(jhtml_suite, "test dt with css 009",                              test_jhtml_dt_tag_with_css_009);
  CU_add_test(jhtml_suite, "test dt with css 010",                              test_jhtml_dt_tag_with_css_010);
  CU_add_test(jhtml_suite, "test dt with css 011",                              test_jhtml_dt_tag_with_css_011);
  CU_add_test(jhtml_suite, "test dt with css 012",                              test_jhtml_dt_tag_with_css_012);
  CU_add_test(jhtml_suite, "test dt with css 013",                              test_jhtml_dt_tag_with_css_013);
  CU_add_test(jhtml_suite, "test dt with css 014",                              test_jhtml_dt_tag_with_css_014);

  CU_add_test(jhtml_suite, "test blockquote with css 001",                       test_jhtml_blockquote_tag_with_css_001);
  CU_add_test(jhtml_suite, "test blockquote with css 002",                       test_jhtml_blockquote_tag_with_css_002);
  CU_add_test(jhtml_suite, "test blockquote with css 003",                       test_jhtml_blockquote_tag_with_css_003);
  CU_add_test(jhtml_suite, "test blockquote with css 004",                       test_jhtml_blockquote_tag_with_css_004);
  CU_add_test(jhtml_suite, "test blockquote with css 005",                       test_jhtml_blockquote_tag_with_css_005);
  CU_add_test(jhtml_suite, "test blockquote with css 006",                       test_jhtml_blockquote_tag_with_css_006);
  CU_add_test(jhtml_suite, "test blockquote with css 007",                       test_jhtml_blockquote_tag_with_css_007);
  CU_add_test(jhtml_suite, "test blockquote with css 008",                       test_jhtml_blockquote_tag_with_css_008);
  CU_add_test(jhtml_suite, "test blockquote with css 009",                       test_jhtml_blockquote_tag_with_css_009);
  CU_add_test(jhtml_suite, "test blockquote with css 010",                       test_jhtml_blockquote_tag_with_css_010);
  CU_add_test(jhtml_suite, "test blockquote with css 011",                       test_jhtml_blockquote_tag_with_css_011);
  CU_add_test(jhtml_suite, "test blockquote with css 012",                       test_jhtml_blockquote_tag_with_css_012);
  CU_add_test(jhtml_suite, "test blockquote with css 013",                       test_jhtml_blockquote_tag_with_css_013);
  CU_add_test(jhtml_suite, "test blockquote with css 014",                       test_jhtml_blockquote_tag_with_css_014);
  CU_add_test(jhtml_suite, "test blockquote with css 015",                       test_jhtml_blockquote_tag_with_css_015);
  CU_add_test(jhtml_suite, "test blockquote with css 016",                       test_jhtml_blockquote_tag_with_css_016);


  CU_add_test(jhtml_suite, "test dir with css 001",                       test_jhtml_dir_tag_with_css_001);
  CU_add_test(jhtml_suite, "test dir with css 002",                       test_jhtml_dir_tag_with_css_002);
  CU_add_test(jhtml_suite, "test dir with css 003",                       test_jhtml_dir_tag_with_css_003);
  CU_add_test(jhtml_suite, "test dir with css 004",                       test_jhtml_dir_tag_with_css_004);
  CU_add_test(jhtml_suite, "test dir with css 005",                       test_jhtml_dir_tag_with_css_005);
  CU_add_test(jhtml_suite, "test dir with css 006",                       test_jhtml_dir_tag_with_css_006);
  CU_add_test(jhtml_suite, "test dir with css 007",                       test_jhtml_dir_tag_with_css_007);
  CU_add_test(jhtml_suite, "test dir with css 008",                       test_jhtml_dir_tag_with_css_008);
  CU_add_test(jhtml_suite, "test dir with css 009",                       test_jhtml_dir_tag_with_css_009);
  CU_add_test(jhtml_suite, "test dir with css 010",                       test_jhtml_dir_tag_with_css_010);
  CU_add_test(jhtml_suite, "test dir with css 011",                       test_jhtml_dir_tag_with_css_011);
  CU_add_test(jhtml_suite, "test dir with css 012",                       test_jhtml_dir_tag_with_css_012);
  CU_add_test(jhtml_suite, "test dir with css 013",                       test_jhtml_dir_tag_with_css_013);
  CU_add_test(jhtml_suite, "test dir with css 014",                       test_jhtml_dir_tag_with_css_014);
  CU_add_test(jhtml_suite, "test dir with css 015",                       test_jhtml_dir_tag_with_css_015);
  CU_add_test(jhtml_suite, "test dir with css 016",                       test_jhtml_dir_tag_with_css_016);
  CU_add_test(jhtml_suite, "test dir with css 017",                       test_jhtml_dir_tag_with_css_017);
  CU_add_test(jhtml_suite, "test dir with css 018",                       test_jhtml_dir_tag_with_css_018);
  CU_add_test(jhtml_suite, "test dir with css 019",                       test_jhtml_dir_tag_with_css_019);
  CU_add_test(jhtml_suite, "test dir with css 020",                       test_jhtml_dir_tag_with_css_020);
  CU_add_test(jhtml_suite, "test dir with css 021",                       test_jhtml_dir_tag_with_css_021);
  CU_add_test(jhtml_suite, "test dir with css 022",                       test_jhtml_dir_tag_with_css_022);
  /* aend */

  CU_basic_run_tests();
  CU_cleanup_registry();

  return(0);
}

void test_log_rerror(const char *file, int line, int level, apr_status_t status, const request_rec *r, const char *fmt, ...)
{
  va_list ap;
  fprintf(stderr, "ERROR LOG %s:%d ", file,line);
  va_start(ap, fmt);
  vfprintf(stderr, fmt,ap);
  va_end(ap);
  fprintf(stderr, "\n");
}
void test_log_error(const char *file, int line, int level, apr_status_t status, const request_rec *r, const char *fmt, ...)
{
  va_list ap;
  fprintf(stderr, "ERROR LOG %s:%d ", file,line);
  va_start(ap, fmt);
  vfprintf(stderr, fmt,ap);
  va_end(ap);
  fprintf(stderr, "\n");
}

char *chxj_qr_code_blob_handler(request_rec* r, const char* indata, size_t* len)
{
  return NULL;
}

char *test_os_escape_path(apr_pool_t *p, const char *path, int partial)
{
  return (char *)path;
}

void test_set_content_type(request_rec *r, const char *ct)
{
  fprintf(stderr, "%s:%d set_content_type(%s)\n", __FILE__,__LINE__,ct);
}

const char *test_run_http_scheme(request_rec *r)
{
  static char *s = "http";
  return s;
}

void * test_get_module_config(const ap_conf_vector_t *cv, const module *m)
{
  static mod_chxj_config cfg;
  memset(&cfg, 0, sizeof(mod_chxj_config));
  cfg.new_line_type = NLTYPE_NONE;
  return &cfg;
}

chxjconvrule_entry *
chxj_apply_convrule(request_rec* r, apr_array_header_t* convrules)
{
  static chxjconvrule_entry entries;
  memset(&entries, 0, sizeof(chxjconvrule_entry));
  entries.encoding = apr_pstrdup(r->pool, "UTF8");
  return &entries;
}

char *test_ap_escape_html(apr_pool_t *pool, const char *s)
{
  return (char *)s;
}

#define APR_INIT \
  request_rec r; \
  apr_pool_t *p; \
  do { \
    apr_initialize(); \
    apr_pool_create(&p, NULL); \
    r.pool = p; \
    r.hostname = apr_pstrdup(p, "localhost"); \
  } \
  while (0)

#define APR_TERM \
  do { \
    apr_terminate(); \
  } while (0)

#define COOKIE_INIT(X) \
  do { \
    X.cookie_id = "test_cookie_id"; \
  } while (0) \

#define SPEC_INIT(X) \
  do { \
    X.html_spec_type = CHXJ_SPEC_Jhtml; \
  } while (0)



/*===========================================================================*/
/* COMMENT                                                                   */
/*===========================================================================*/
void test_jhtml_001() 
{
#define  TEST_STRING "<!-- comment -->"
#define TEST_RESULT "\n"
  char  *ret;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);

  ret = chxj_convert_jhtml(&r, &spec, TEST_STRING, sizeof(TEST_STRING)-1, &destlen, &entry, &cookie);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", TEST_RESULT);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp("\n", ret) == 0);
  CU_ASSERT(destlen == 1);

  APR_TERM;
#undef TEST_STRING
}
void test_jhtml_002() 
{
#define  TEST_STRING ""
#define TEST_RESULT "\n"
  char  *ret;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);

  ret = chxj_convert_jhtml(&r, &spec, TEST_STRING, sizeof(TEST_STRING)-1, &destlen, &entry, &cookie);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", TEST_RESULT);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp("\n", ret) == 0);
  CU_ASSERT(destlen == 1);

  APR_TERM;
#undef TEST_STRING
}
void test_jhtml_comment_tag_001() 
{
#define  TEST_STRING "<html><!--</html><body>--><head></head><body></body></html>"
#define  RESULT_STRING "<html><head></head><body></body></html>"
  char  *ret;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);

  ret = chxj_convert_jhtml(&r, &spec, TEST_STRING, sizeof(TEST_STRING)-1, &destlen, &entry, &cookie);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* <A>                                                                        */
/*============================================================================*/
void test_jhtml_a_tag_name_attribute_001() 
{
#define  TEST_STRING "<a name=\"abc\">abc</a>"
#define  RESULT_STRING "<a name=\"abc\">abc</a>"
  char  *ret;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);

  ret = chxj_convert_jhtml(&r, &spec, TEST_STRING, sizeof(TEST_STRING)-1, &destlen, &entry, &cookie);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_a_tag_name_attribute_002() 
{
#define  TEST_STRING "<a name=\"\">abc</a>"
#define  RESULT_STRING "<a name=\"\">abc</a>"
  char  *ret;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);

  ret = chxj_convert_jhtml(&r, &spec, TEST_STRING, sizeof(TEST_STRING)-1, &destlen, &entry, &cookie);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_a_tag_name_attribute_003() 
{
#define  TEST_STRING "<a name=\"あああ\">abc</a>"
#define  RESULT_STRING "<a name=\"あああ\">abc</a>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);

  destlen = sizeof(TEST_STRING)-1;
  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_a_tag_name_attribute_004() 
{
#define  TEST_STRING "<a name=\"ｱｱｱ\">abc</a>"
#define  RESULT_STRING "<a name=\"ｱｱｱ\">abc</a>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_a_tag_href_attribute_001() 
{
#define  TEST_STRING "<a href=\"\">abc</a>"
#define  RESULT_STRING "<a href=\"?_chxj_cc=test_cookie_id\">abc</a>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_a_tag_href_attribute_002() 
{
#define  TEST_STRING "<html><head></head><body><a href=\"http://www.google.co.jp/\">abc</a></body></html>"
#define  RESULT_STRING "<html><head></head><body><a href=\"http://www.google.co.jp/\">abc</a></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_a_tag_href_attribute_003() 
{
#define  TEST_STRING "<html><head></head><body><a href=\"a.html\">abc</a></body></html>"
#define  RESULT_STRING "<html><head></head><body><a href=\"a.html?_chxj_cc=test_cookie_id\">abc</a></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_a_tag_href_attribute_004() 
{
#define  TEST_STRING "<html><head></head><body><a href=\"a.html#abc\">abc</a></body></html>"
#define  RESULT_STRING "<html><head></head><body><a href=\"a.html?_chxj_cc=test_cookie_id#abc\">abc</a></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_a_tag_href_attribute_005() 
{
#define  TEST_STRING "<html><head></head><body><a href=\"a.html#\">abc</a></body></html>"
#define  RESULT_STRING "<html><head></head><body><a href=\"a.html?_chxj_cc=test_cookie_id#\">abc</a></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_a_tag_href_attribute_006() 
{
#define  TEST_STRING "<html><head></head><body><a href=\"a.html\">abc</a></body></html>"
#define  RESULT_STRING "<html><head></head><body><a href=\"a.html\">abc</a></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, NULL);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_a_tag_accesskey_attribute_001() 
{
#define  TEST_STRING "<html><head></head><body><a accesskey=\"1\">abc</a></body></html>"
#define  RESULT_STRING "<html><head></head><body><a accesskey=\"1\">abc</a></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_a_tag_accesskey_attribute_002() 
{
#define  TEST_STRING "<html><head></head><body><a accesskey=\"\">abc</a></body></html>"
#define  RESULT_STRING "<html><head></head><body><a accesskey=\"\">abc</a></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_a_tag_accesskey_attribute_003() 
{
#define  TEST_STRING "<html><head></head><body><a accesskey>abc</a></body></html>"
#define  RESULT_STRING "<html><head></head><body><a accesskey=\"\">abc</a></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* <BASE>                                                                     */
/*============================================================================*/
void test_jhtml_base_tag_001() 
{
#define  TEST_STRING "<html><head><base></head><body></body></html>"
#define  RESULT_STRING "<html><head><base></head><body></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_base_tag_href_attribute_001() 
{
#define  TEST_STRING "<html><head><base href></head><body></body></html>"
#define  RESULT_STRING "<html><head><base href=\"\"></head><body></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_base_tag_href_attribute_002() 
{
#define  TEST_STRING "<html><head><base href=\"\"></head><body></body></html>"
#define  RESULT_STRING "<html><head><base href=\"\"></head><body></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_base_tag_href_attribute_003() 
{
#define  TEST_STRING "<html><head><base href=\"http://www.google.co.jp/\"></head><body></body></html>"
#define  RESULT_STRING "<html><head><base href=\"http://www.google.co.jp/\"></head><body></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_base_tag_href_attribute_004() 
{
#define  TEST_STRING "<html><head><base href=\".\"></head><body></body></html>"
#define  RESULT_STRING "<html><head><base href=\".\"></head><body></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* <BLOCKQUOTE>                                                               */
/*============================================================================*/
void test_jhtml_blockquote_tag_001() 
{
#define  TEST_STRING "<html><head></head><body><blockquote></blockquote></body></html>"
#define  RESULT_STRING "<html><head></head><body><blockquote></blockquote></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_blockquote_tag_002() 
{
#define  TEST_STRING "<html><head></head><body><blockquote>abc</blockquote></body></html>"
#define  RESULT_STRING "<html><head></head><body><blockquote>abc</blockquote></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_blockquote_tag_003() 
{
#define  TEST_STRING "<html><head></head><body><blockquote>亀さん</blockquote></body></html>"
#define  RESULT_STRING "<html><head></head><body><blockquote>亀さん</blockquote></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_blockquote_tag_004() 
{
#define  TEST_STRING "<html><head></head><body><blockquote>ﾊﾝｶｸ</blockquote></body></html>"
#define  RESULT_STRING "<html><head></head><body><blockquote>ﾊﾝｶｸ</blockquote></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* <BODY>                                                                     */
/*============================================================================*/
void test_jhtml_body_tag_001() 
{
#define  TEST_STRING "<html><head></head><body></body></html>"
#define  RESULT_STRING "<html><head></head><body></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_body_tag_002() 
{
#define  TEST_STRING "<html><head></head><body bgcolor></body></html>"
#define  RESULT_STRING "<html><head></head><body></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_body_tag_003() 
{
#define  TEST_STRING "<html><head></head><body bgcolor=\"\"></body></html>"
#define  RESULT_STRING "<html><head></head><body></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_body_tag_004() 
{
#define  TEST_STRING "<html><head></head><body bgcolor=\"#ff0000\"></body></html>"
#define  RESULT_STRING "<html><head></head><body bgcolor=\"#ff0000\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_body_tag_005() 
{
#define  TEST_STRING "<html><head></head><body text></body></html>"
#define  RESULT_STRING "<html><head></head><body></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_body_tag_006() 
{
#define  TEST_STRING "<html><head></head><body text=\"\"></body></html>"
#define  RESULT_STRING "<html><head></head><body></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_body_tag_007() 
{
#define  TEST_STRING "<html><head></head><body text=\"#ff0000\"></body></html>"
#define  RESULT_STRING "<html><head></head><body text=\"#ff0000\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_body_tag_008() 
{
#define  TEST_STRING "<html><head></head><body link></body></html>"
#define  RESULT_STRING "<html><head></head><body></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_body_tag_009() 
{
#define  TEST_STRING "<html><head></head><body link=\"\"></body></html>"
#define  RESULT_STRING "<html><head></head><body></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_body_tag_010() 
{
#define  TEST_STRING "<html><head></head><body link=\"#ff0000\"></body></html>"
#define  RESULT_STRING "<html><head></head><body link=\"#ff0000\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_body_tag_011() 
{
#define  TEST_STRING "<html><head></head><body vlink></body></html>"
#define  RESULT_STRING "<html><head></head><body></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_body_tag_012() 
{
#define  TEST_STRING "<html><head></head><body vlink=\"\"></body></html>"
#define  RESULT_STRING "<html><head></head><body></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_body_tag_013() 
{
#define  TEST_STRING "<html><head></head><body vlink=\"#ff0000\"></body></html>"
#define  RESULT_STRING "<html><head></head><body vlink=\"#ff0000\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_body_tag_014() 
{
#define  TEST_STRING "<html><head></head><body alink=\"#FF0000\"></body></html>"
#define  RESULT_STRING "<html><head></head><body></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_body_tag_015() 
{
#define  TEST_STRING "<html><head></head><body boyoyon=\"#FF0000\"></body></html>"
#define  RESULT_STRING "<html><head></head><body></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* <BR>                                                                       */
/*============================================================================*/
void test_jhtml_br_tag_001() 
{
#define  TEST_STRING "<html><head></head><body><br></body></html>"
#define  RESULT_STRING "<html><head></head><body><br></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_br_tag_002() 
{
#define  TEST_STRING "<html><head></head><body><br clear=\"left\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><br clear=\"left\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_br_tag_003() 
{
#define  TEST_STRING "<html><head></head><body><br clear=\"right\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><br clear=\"right\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_br_tag_004() 
{
#define  TEST_STRING "<html><head></head><body><br clear=\"all\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><br clear=\"all\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_br_tag_005() 
{
#define  TEST_STRING "<html><head></head><body><br clear=\"\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><br></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_br_tag_006() 
{
#define  TEST_STRING "<html><head></head><body><br clear></body></html>"
#define  RESULT_STRING "<html><head></head><body><br></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_br_tag_007() 
{
#define  TEST_STRING "<html><head></head><body><br clear=\"abc\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><br></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* <CENTER>                                                                   */
/*============================================================================*/
void test_jhtml_center_tag_001()
{
#define  TEST_STRING "<html><head></head><body><center>あいうえお</center></body></html>"
#define  RESULT_STRING "<html><head></head><body><center>あいうえお</center></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* <DIR>                                                                      */
/*============================================================================*/
void test_jhtml_dir_tag_001()
{
#define  TEST_STRING "<html><head></head><body><dir><li>あああ</li><li>いいい</li></dir></body></html>"
#define  RESULT_STRING "<html><head></head><body><dir><li>あああ</li><li>いいい</li></dir></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_dir_tag_002()
{
#define  TEST_STRING "<html><head></head><body><dir></dir></body></html>"
#define  RESULT_STRING "<html><head></head><body><dir></dir></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_dir_tag_003()
{
#define  TEST_STRING "<html><head></head><body><dir type=\"disc\"></dir></body></html>"
#define  RESULT_STRING "<html><head></head><body><dir></dir></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* <DL>                                                                       */
/*============================================================================*/
void test_jhtml_dl_tag_001()
{
#define  TEST_STRING "<html><head></head><body><dl></dl></body></html>"
#define  RESULT_STRING "<html><head></head><body><dl></dl></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* <DT>                                                                       */
/*============================================================================*/
void test_jhtml_dt_tag_001()
{
#define  TEST_STRING "<html><head></head><body><dl><dt>あああ</dt><dt>いいい</dt></dl></body></html>"
#define  RESULT_STRING "<html><head></head><body><dl><dt>あああ<dt>いいい</dl></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_dt_tag_002()
{
#define  TEST_STRING "<html><head></head><body><dl><dt></dt><dt></dt></dl></body></html>"
#define  RESULT_STRING "<html><head></head><body><dl><dt><dt></dl></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* <DD>                                                                       */
/*============================================================================*/
void test_jhtml_dd_tag_001()
{
#define  TEST_STRING "<html><head></head><body><dl><dt>あああ</dt><dd>いいい</dd><dt>ああああ</dt><dd>いいいい</dd></dl></body></html>"
#define  RESULT_STRING "<html><head></head><body><dl><dt>あああ<dd>いいい<dt>ああああ<dd>いいいい</dl></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_dd_tag_002()
{
#define  TEST_STRING "<html><head></head><body><dl><dd></dd><dd></dd></dl></body></html>"
#define  RESULT_STRING "<html><head></head><body><dl><dd><dd></dl></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* <DIV>                                                                      */
/*============================================================================*/
void test_jhtml_div_tag_001() 
{
#define  TEST_STRING "<html><head></head><body><div></div></body></html>"
#define  RESULT_STRING "<html><head></head><body></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_div_tag_002() 
{
#define  TEST_STRING "<html><head></head><body><div align=\"left\"></div></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"left\"></div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_div_tag_003() 
{
#define  TEST_STRING "<html><head></head><body><div align=\"right\"></div></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"right\"></div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_div_tag_004() 
{
#define  TEST_STRING "<html><head></head><body><div align=\"center\"></div></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"center\"></div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_div_tag_005() 
{
#define  TEST_STRING "<html><head></head><body><div align=\"\"></div></body></html>"
#define  RESULT_STRING "<html><head></head><body></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_div_tag_006() 
{
#define  TEST_STRING "<html><head></head><body><div align=\"abc\"></div></body></html>"
#define  RESULT_STRING "<html><head></head><body></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_div_tag_007() 
{
#define  TEST_STRING "<html><head></head><body><div style=\"aaa\"></div></body></html>"
#define  RESULT_STRING "<html><head></head><body></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* FORM                                                                       */
/*============================================================================*/
void test_jhtml_form_tag_001() 
{
#define  TEST_STRING "<html><head></head><body><form></form></body></html>"
#define  RESULT_STRING "<html><head></head><body><form></form></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_form_tag_002() 
{
#define  TEST_STRING "<html><head></head><body><form method></form></body></html>"
#define  RESULT_STRING "<html><head></head><body><form method=\"\"></form></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_form_tag_003() 
{
#define  TEST_STRING "<html><head></head><body><form method=\"post\"></form></body></html>"
#define  RESULT_STRING "<html><head></head><body><form method=\"post\"></form></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_form_tag_004() 
{
#define  TEST_STRING "<html><head></head><body><form method=\"get\"></form></body></html>"
#define  RESULT_STRING "<html><head></head><body><form method=\"get\"></form></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_form_tag_005() 
{
#define  TEST_STRING "<html><head></head><body><form method=\"abc\"></form></body></html>"
#define  RESULT_STRING "<html><head></head><body><form method=\"abc\"></form></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_form_tag_006() 
{
#define  TEST_STRING "<html><head></head><body><form action></form></body></html>"
#define  RESULT_STRING "<html><head></head><body><form action=\"\"><input type=\"hidden\" name=\"_chxj_cc\" value=\"test_cookie_id\"></form></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_form_tag_007() 
{
#define  TEST_STRING "<html><head></head><body><form action></form></body></html>"
#define  RESULT_STRING "<html><head></head><body><form action=\"\"></form></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, NULL);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_form_tag_008() 
{
#define  TEST_STRING "<html><head></head><body><form action=\"http://www.google.co.jp/\"></form></body></html>"
#define  RESULT_STRING "<html><head></head><body><form action=\"http://www.google.co.jp/\"></form></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_form_tag_009() 
{
#define  TEST_STRING "<html><head></head><body><form method=\"post\" action=\"hogehoge\"></form></body></html>"
#define  RESULT_STRING "<html><head></head><body><form action=\"hogehoge\" method=\"post\"><input type=\"hidden\" name=\"_chxj_cc\" value=\"test_cookie_id\"></form></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* <HEAD>                                                                     */
/*============================================================================*/
void test_jhtml_head_tag_001()
{
#define  TEST_STRING "<html><head></head><body></body></html>"
#define  RESULT_STRING "<html><head></head><body></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_head_tag_002()
{
#define  TEST_STRING "<html><head><title>あああ</title></head><body></body></html>"
#define  RESULT_STRING "<html><head><title>あああ</title></head><body></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* <H>                                                                        */
/*============================================================================*/
void test_jhtml_h1_tag_001() 
{
#define  TEST_STRING "<html><head></head><body><h1></h1></body></html>"
#define  RESULT_STRING "<html><head></head><body></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h1_tag_002() 
{
#define  TEST_STRING "<html><head></head><body><h1>abc</h1></body></html>"
#define  RESULT_STRING "<html><head></head><body>abc</body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h1_tag_003() 
{
#define  TEST_STRING "<html><head></head><body><h1>亀さん</h1></body></html>"
#define  RESULT_STRING "<html><head></head><body>亀さん</body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h1_tag_004() 
{
#define  TEST_STRING "<html><head></head><body><h1>ﾊﾝｶｸ</h1></body></html>"
#define  RESULT_STRING "<html><head></head><body>ﾊﾝｶｸ</body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h1_tag_005() 
{
#define  TEST_STRING "<html><head></head><body><h1 align></h1></body></html>"
#define  RESULT_STRING "<html><head></head><body></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h1_tag_006() 
{
#define  TEST_STRING "<html><head></head><body><h1 align=\"\"></h1></body></html>"
#define  RESULT_STRING "<html><head></head><body></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h1_tag_007() 
{
#define  TEST_STRING "<html><head></head><body><h1 align=\"left\"></h1></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"left\"></div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h1_tag_008() 
{
#define  TEST_STRING "<html><head></head><body><h1 align=\"right\"></h1></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"right\"></div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h1_tag_009() 
{
#define  TEST_STRING "<html><head></head><body><h1 align=\"center\"></h1></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"center\"></div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h1_tag_010() 
{
#define  TEST_STRING "<html><head></head><body><h1 align=\"unkown\"></h1></body></html>"
#define  RESULT_STRING "<html><head></head><body></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*===========================================================================*/
/* <h2>                                                                      */
/*===========================================================================*/
void test_jhtml_h2_tag_001() 
{
#define  TEST_STRING "<html><head></head><body><h2></h2></body></html>"
#define  RESULT_STRING "<html><head></head><body></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h2_tag_002() 
{
#define  TEST_STRING "<html><head></head><body><h2>abc</h2></body></html>"
#define  RESULT_STRING "<html><head></head><body>abc</body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h2_tag_003() 
{
#define  TEST_STRING "<html><head></head><body><h2>亀さん</h2></body></html>"
#define  RESULT_STRING "<html><head></head><body>亀さん</body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h2_tag_004() 
{
#define  TEST_STRING "<html><head></head><body><h2>ﾊﾝｶｸ</h2></body></html>"
#define  RESULT_STRING "<html><head></head><body>ﾊﾝｶｸ</body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h2_tag_005() 
{
#define  TEST_STRING "<html><head></head><body><h2 align></h2></body></html>"
#define  RESULT_STRING "<html><head></head><body></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h2_tag_006() 
{
#define  TEST_STRING "<html><head></head><body><h2 align=\"\"></h2></body></html>"
#define  RESULT_STRING "<html><head></head><body></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h2_tag_007() 
{
#define  TEST_STRING "<html><head></head><body><h2 align=\"left\"></h2></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"left\"></div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h2_tag_008() 
{
#define  TEST_STRING "<html><head></head><body><h2 align=\"right\"></h2></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"right\"></div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h2_tag_009() 
{
#define  TEST_STRING "<html><head></head><body><h2 align=\"center\"></h2></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"center\"></div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h2_tag_010() 
{
#define  TEST_STRING "<html><head></head><body><h2 align=\"unkown\"></h2></body></html>"
#define  RESULT_STRING "<html><head></head><body></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*===========================================================================*/
/* <h3>                                                                      */
/*===========================================================================*/
void test_jhtml_h3_tag_001() 
{
#define  TEST_STRING "<html><head></head><body><h3></h3></body></html>"
#define  RESULT_STRING "<html><head></head><body></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h3_tag_002() 
{
#define  TEST_STRING "<html><head></head><body><h3>abc</h3></body></html>"
#define  RESULT_STRING "<html><head></head><body>abc</body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h3_tag_003() 
{
#define  TEST_STRING "<html><head></head><body><h3>亀さん</h3></body></html>"
#define  RESULT_STRING "<html><head></head><body>亀さん</body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h3_tag_004() 
{
#define  TEST_STRING "<html><head></head><body><h3>ﾊﾝｶｸ</h3></body></html>"
#define  RESULT_STRING "<html><head></head><body>ﾊﾝｶｸ</body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h3_tag_005() 
{
#define  TEST_STRING "<html><head></head><body><h3 align></h3></body></html>"
#define  RESULT_STRING "<html><head></head><body></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h3_tag_006() 
{
#define  TEST_STRING "<html><head></head><body><h3 align=\"\"></h3></body></html>"
#define  RESULT_STRING "<html><head></head><body></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h3_tag_007() 
{
#define  TEST_STRING "<html><head></head><body><h3 align=\"left\"></h3></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"left\"></div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h3_tag_008() 
{
#define  TEST_STRING "<html><head></head><body><h3 align=\"right\"></h3></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"right\"></div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h3_tag_009() 
{
#define  TEST_STRING "<html><head></head><body><h3 align=\"center\"></h3></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"center\"></div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h3_tag_010() 
{
#define  TEST_STRING "<html><head></head><body><h3 align=\"unkown\"></h3></body></html>"
#define  RESULT_STRING "<html><head></head><body></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*===========================================================================*/
/* <h4>                                                                      */
/*===========================================================================*/
void test_jhtml_h4_tag_001() 
{
#define  TEST_STRING "<html><head></head><body><h4></h4></body></html>"
#define  RESULT_STRING "<html><head></head><body></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h4_tag_002() 
{
#define  TEST_STRING "<html><head></head><body><h4>abc</h4></body></html>"
#define  RESULT_STRING "<html><head></head><body>abc</body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h4_tag_003() 
{
#define  TEST_STRING "<html><head></head><body><h4>亀さん</h4></body></html>"
#define  RESULT_STRING "<html><head></head><body>亀さん</body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h4_tag_004() 
{
#define  TEST_STRING "<html><head></head><body><h4>ﾊﾝｶｸ</h4></body></html>"
#define  RESULT_STRING "<html><head></head><body>ﾊﾝｶｸ</body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h4_tag_005() 
{
#define  TEST_STRING "<html><head></head><body><h4 align></h4></body></html>"
#define  RESULT_STRING "<html><head></head><body></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h4_tag_006() 
{
#define  TEST_STRING "<html><head></head><body><h4 align=\"\"></h4></body></html>"
#define  RESULT_STRING "<html><head></head><body></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h4_tag_007() 
{
#define  TEST_STRING "<html><head></head><body><h4 align=\"left\"></h4></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"left\"></div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h4_tag_008() 
{
#define  TEST_STRING "<html><head></head><body><h4 align=\"right\"></h4></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"right\"></div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h4_tag_009() 
{
#define  TEST_STRING "<html><head></head><body><h4 align=\"center\"></h4></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"center\"></div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h4_tag_010() 
{
#define  TEST_STRING "<html><head></head><body><h4 align=\"unkown\"></h4></body></html>"
#define  RESULT_STRING "<html><head></head><body></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*===========================================================================*/
/* <h5>                                                                      */
/*===========================================================================*/
void test_jhtml_h5_tag_001() 
{
#define  TEST_STRING "<html><head></head><body><h5></h5></body></html>"
#define  RESULT_STRING "<html><head></head><body></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h5_tag_002() 
{
#define  TEST_STRING "<html><head></head><body><h5>abc</h5></body></html>"
#define  RESULT_STRING "<html><head></head><body>abc</body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h5_tag_003() 
{
#define  TEST_STRING "<html><head></head><body><h5>亀さん</h5></body></html>"
#define  RESULT_STRING "<html><head></head><body>亀さん</body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h5_tag_004() 
{
#define  TEST_STRING "<html><head></head><body><h5>ﾊﾝｶｸ</h5></body></html>"
#define  RESULT_STRING "<html><head></head><body>ﾊﾝｶｸ</body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h5_tag_005() 
{
#define  TEST_STRING "<html><head></head><body><h5 align></h5></body></html>"
#define  RESULT_STRING "<html><head></head><body></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h5_tag_006() 
{
#define  TEST_STRING "<html><head></head><body><h5 align=\"\"></h5></body></html>"
#define  RESULT_STRING "<html><head></head><body></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h5_tag_007() 
{
#define  TEST_STRING "<html><head></head><body><h5 align=\"left\"></h5></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"left\"></div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h5_tag_008() 
{
#define  TEST_STRING "<html><head></head><body><h5 align=\"right\"></h5></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"right\"></div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h5_tag_009() 
{
#define  TEST_STRING "<html><head></head><body><h5 align=\"center\"></h5></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"center\"></div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h5_tag_010() 
{
#define  TEST_STRING "<html><head></head><body><h5 align=\"unkown\"></h5></body></html>"
#define  RESULT_STRING "<html><head></head><body></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*===========================================================================*/
/* <h6>                                                                      */
/*===========================================================================*/
void test_jhtml_h6_tag_001() 
{
#define  TEST_STRING "<html><head></head><body><h6></h6></body></html>"
#define  RESULT_STRING "<html><head></head><body></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h6_tag_002() 
{
#define  TEST_STRING "<html><head></head><body><h6>abc</h6></body></html>"
#define  RESULT_STRING "<html><head></head><body>abc</body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h6_tag_003() 
{
#define  TEST_STRING "<html><head></head><body><h6>亀さん</h6></body></html>"
#define  RESULT_STRING "<html><head></head><body>亀さん</body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h6_tag_004() 
{
#define  TEST_STRING "<html><head></head><body><h6>ﾊﾝｶｸ</h6></body></html>"
#define  RESULT_STRING "<html><head></head><body>ﾊﾝｶｸ</body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h6_tag_005() 
{
#define  TEST_STRING "<html><head></head><body><h6 align></h6></body></html>"
#define  RESULT_STRING "<html><head></head><body></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h6_tag_006() 
{
#define  TEST_STRING "<html><head></head><body><h6 align=\"\"></h6></body></html>"
#define  RESULT_STRING "<html><head></head><body></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h6_tag_007() 
{
#define  TEST_STRING "<html><head></head><body><h6 align=\"left\"></h6></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"left\"></div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h6_tag_008() 
{
#define  TEST_STRING "<html><head></head><body><h6 align=\"right\"></h6></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"right\"></div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h6_tag_009() 
{
#define  TEST_STRING "<html><head></head><body><h6 align=\"center\"></h6></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"center\"></div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h6_tag_010() 
{
#define  TEST_STRING "<html><head></head><body><h6 align=\"unkown\"></h6></body></html>"
#define  RESULT_STRING "<html><head></head><body></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* <HR>                                                                       */
/*============================================================================*/
void test_jhtml_hr_tag_001() 
{
#define  TEST_STRING "<html><head></head><body><hr></body></html>"
#define  RESULT_STRING "<html><head></head><body><hr></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_hr_tag_002() 
{
#define  TEST_STRING "<html><head></head><body><hr /></body></html>"
#define  RESULT_STRING "<html><head></head><body><hr></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_hr_tag_003() 
{
#define  TEST_STRING "<html><head></head><body><hr align></body></html>"
#define  RESULT_STRING "<html><head></head><body><hr></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_hr_tag_004() 
{
#define  TEST_STRING "<html><head></head><body><hr align=\"\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><hr></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_hr_tag_005() 
{
#define  TEST_STRING "<html><head></head><body><hr align=\"left\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><hr align=\"left\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_hr_tag_006() 
{
#define  TEST_STRING "<html><head></head><body><hr align=\"right\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><hr align=\"right\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_hr_tag_007() 
{
#define  TEST_STRING "<html><head></head><body><hr align=\"center\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><hr align=\"center\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_hr_tag_008() 
{
#define  TEST_STRING "<html><head></head><body><hr align=\"unknown\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><hr></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_hr_tag_009() 
{
#define  TEST_STRING "<html><head></head><body><hr size></body></html>"
#define  RESULT_STRING "<html><head></head><body><hr></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_hr_tag_010() 
{
#define  TEST_STRING "<html><head></head><body><hr size=\"\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><hr></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_hr_tag_011() 
{
#define  TEST_STRING "<html><head></head><body><hr size=\"abc\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><hr size=\"abc\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_hr_tag_012() 
{
#define  TEST_STRING "<html><head></head><body><hr size=\"10\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><hr size=\"10\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_hr_tag_013() 
{
#define  TEST_STRING "<html><head></head><body><hr width></body></html>"
#define  RESULT_STRING "<html><head></head><body><hr></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_hr_tag_014() 
{
#define  TEST_STRING "<html><head></head><body><hr width=\"\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><hr></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_hr_tag_015() 
{
#define  TEST_STRING "<html><head></head><body><hr width=\"abc\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><hr width=\"abc\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_hr_tag_016() 
{
#define  TEST_STRING "<html><head></head><body><hr width=\"10\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><hr width=\"10\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_hr_tag_017() 
{
#define  TEST_STRING "<html><head></head><body><hr width=\"10\" noshade></body></html>"
#define  RESULT_STRING "<html><head></head><body><hr width=\"10\" noshade></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_hr_tag_018() 
{
#define  TEST_STRING "<html><head></head><body><hr width=\"10\" color=\"#ff0000\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><hr width=\"10\" color=\"#ff0000\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_hr_tag_018_1() 
{
#define  TEST_STRING   "<hr width=\"10\" color=\"\">"
#define  RESULT_STRING "<hr width=\"10\">"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_hr_tag_018_2() 
{
#define  TEST_STRING   "<hr width=\"10\" color>"
#define  RESULT_STRING "<hr width=\"10\">"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* <HTML>                                                                     */
/*============================================================================*/
void test_jhtml_html_tag_001()
{
#define  TEST_STRING "<html><head></head><body></body></html>"
#define  RESULT_STRING "<html><head></head><body></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* <IMG>                                                                      */
/*============================================================================*/
void test_jhtml_img_tag_001() 
{
#define  TEST_STRING "<html><head></head><body><img></body></html>"
#define  RESULT_STRING "<html><head></head><body><img></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_img_tag_002() 
{
#define  TEST_STRING "<html><head></head><body><img src></body></html>"
#define  RESULT_STRING "<html><head></head><body><img src=\"?_chxj_cc=test_cookie_id&_chxj_nc=true\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_img_tag_003() 
{
#define  TEST_STRING "<html><head></head><body><img src=\"\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><img src=\"?_chxj_cc=test_cookie_id&_chxj_nc=true\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_img_tag_004() 
{
#define  TEST_STRING "<html><head></head><body><img src=\"aaa\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><img src=\"aaa?_chxj_cc=test_cookie_id&_chxj_nc=true\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_img_tag_005() 
{
#define  TEST_STRING "<html><head></head><body><img src=\"亀さん\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><img src=\"亀さん?_chxj_cc=test_cookie_id&_chxj_nc=true\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_img_tag_006() 
{
#define  TEST_STRING "<html><head></head><body><img src=\"ﾊﾝｶｸ\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><img src=\"ﾊﾝｶｸ?_chxj_cc=test_cookie_id&_chxj_nc=true\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_img_tag_007() 
{
#define  TEST_STRING "<html><head></head><body><img align></body></html>"
#define  RESULT_STRING "<html><head></head><body><img></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_img_tag_008() 
{
#define  TEST_STRING "<html><head></head><body><img align=\"\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><img></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_img_tag_009() 
{
#define  TEST_STRING "<html><head></head><body><img align=\"top\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><img align=\"top\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_img_tag_010() 
{
#define  TEST_STRING "<html><head></head><body><img align=\"middle\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><img align=\"middle\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_img_tag_011() 
{
#define  TEST_STRING "<html><head></head><body><img align=\"bottom\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><img align=\"bottom\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_img_tag_012() 
{
#define  TEST_STRING "<html><head></head><body><img align=\"left\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><img align=\"left\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_img_tag_013() 
{
#define  TEST_STRING "<html><head></head><body><img align=\"right\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><img align=\"right\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_img_tag_013_1() 
{
#define  TEST_STRING "<html><head></head><body><img align=\"center\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><img align=\"middle\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_img_tag_014() 
{
#define  TEST_STRING "<html><head></head><body><img align=\"unkown\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><img></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_img_tag_015() 
{
#define  TEST_STRING "<html><head></head><body><img width></body></html>"
#define  RESULT_STRING "<html><head></head><body><img></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_img_tag_016() 
{
#define  TEST_STRING "<html><head></head><body><img width=\"\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><img></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_img_tag_017() 
{
#define  TEST_STRING "<html><head></head><body><img width=\"abc\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><img width=\"abc\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_img_tag_018() 
{
#define  TEST_STRING "<html><head></head><body><img width=\"10\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><img width=\"10\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_img_tag_019() 
{
#define  TEST_STRING "<html><head></head><body><img width=\"10%\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><img width=\"10%\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_img_tag_020() 
{
#define  TEST_STRING "<html><head></head><body><img height></body></html>"
#define  RESULT_STRING "<html><head></head><body><img></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_img_tag_021() 
{
#define  TEST_STRING "<html><head></head><body><img height=\"\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><img></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_img_tag_022() 
{
#define  TEST_STRING "<html><head></head><body><img height=\"abc\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><img height=\"abc\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_img_tag_023() 
{
#define  TEST_STRING "<html><head></head><body><img height=\"10\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><img height=\"10\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_img_tag_024() 
{
#define  TEST_STRING "<html><head></head><body><img height=\"10%\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><img height=\"10%\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_img_tag_025() 
{
#define  TEST_STRING "<html><head></head><body><img hspace></body></html>"
#define  RESULT_STRING "<html><head></head><body><img></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_img_tag_026() 
{
#define  TEST_STRING "<html><head></head><body><img hspace=\"\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><img></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_img_tag_027() 
{
#define  TEST_STRING "<html><head></head><body><img hspace=\"abc\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><img></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_img_tag_028() 
{
#define  TEST_STRING "<html><head></head><body><img hspace=\"10\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><img></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_img_tag_029() 
{
#define  TEST_STRING "<html><head></head><body><img vspace></body></html>"
#define  RESULT_STRING "<html><head></head><body><img></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_img_tag_030() 
{
#define  TEST_STRING "<html><head></head><body><img vspace=\"\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><img></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_img_tag_031() 
{
#define  TEST_STRING "<html><head></head><body><img vspace=\"abc\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><img></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_img_tag_032() 
{
#define  TEST_STRING "<html><head></head><body><img vspace=\"10\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><img></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_img_tag_033() 
{
#define  TEST_STRING "<html><head></head><body><img alt></body></html>"
#define  RESULT_STRING "<html><head></head><body><img></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_img_tag_034() 
{
#define  TEST_STRING "<html><head></head><body><img alt=\"\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><img></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_img_tag_035() 
{
#define  TEST_STRING "<html><head></head><body><img alt=\"abc\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><img alt=\"abc\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_img_tag_036() 
{
#define  TEST_STRING "<html><head></head><body><img alt=\"亀さん\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><img alt=\"亀さん\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_img_tag_037() 
{
#define  TEST_STRING "<html><head></head><body><img alt=\"ﾊﾝｶｸ\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><img alt=\"ﾊﾝｶｸ\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* <INPUT>                                                                    */
/*============================================================================*/
void test_jhtml_input_tag_001() 
{
#define  TEST_STRING "<html><head></head><body><input></body></html>"
#define  RESULT_STRING "<html><head></head><body><input></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_input_tag_002() 
{
#define  TEST_STRING "<html><head></head><body><input type></body></html>"
#define  RESULT_STRING "<html><head></head><body><input></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_input_tag_003() 
{
#define  TEST_STRING "<html><head></head><body><input type=\"\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_input_tag_004() 
{
#define  TEST_STRING "<html><head></head><body><input type=\"text\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input type=\"text\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_input_tag_005() 
{
#define  TEST_STRING "<html><head></head><body><input type=\"password\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input type=\"password\" mode=\"numeric\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_input_tag_006() 
{
#define  TEST_STRING "<html><head></head><body><input type=\"checkbox\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input type=\"checkbox\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_input_tag_007() 
{
#define  TEST_STRING "<html><head></head><body><input type=\"radio\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input type=\"radio\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_input_tag_008() 
{
#define  TEST_STRING "<html><head></head><body><input type=\"hidden\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input type=\"hidden\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_input_tag_009() 
{
#define  TEST_STRING "<html><head></head><body><input type=\"submit\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input type=\"submit\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_input_tag_010() 
{
#define  TEST_STRING "<html><head></head><body><input type=\"reset\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input type=\"reset\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_input_tag_011() 
{
#define  TEST_STRING "<html><head></head><body><input type=\"unknown\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_input_tag_012() 
{
#define  TEST_STRING "<html><head></head><body><input name></body></html>"
#define  RESULT_STRING "<html><head></head><body><input></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_input_tag_013() 
{
#define  TEST_STRING "<html><head></head><body><input name=\"\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_input_tag_014() 
{
#define  TEST_STRING "<html><head></head><body><input name=\"a\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input name=\"a\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_input_tag_015() 
{
#define  TEST_STRING "<html><head></head><body><input name=\"亀\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input name=\"亀\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_input_tag_016() 
{
#define  TEST_STRING "<html><head></head><body><input name=\"ﾊﾝｶｸ\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input name=\"ﾊﾝｶｸ\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_input_tag_017() 
{
#define  TEST_STRING "<html><head></head><body><input value></body></html>"
#define  RESULT_STRING "<html><head></head><body><input></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_input_tag_018() 
{
#define  TEST_STRING "<html><head></head><body><input value=\"\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_input_tag_019() 
{
#define  TEST_STRING "<html><head></head><body><input value=\"a\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input value=\"a\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_input_tag_020() 
{
#define  TEST_STRING "<html><head></head><body><input value=\"亀\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input value=\"亀\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_input_tag_021() 
{
#define  TEST_STRING "<html><head></head><body><input value=\"ﾊﾝｶｸ\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input value=\"ﾊﾝｶｸ\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_input_tag_022() 
{
#define  TEST_STRING "<html><head></head><body><input size></body></html>"
#define  RESULT_STRING "<html><head></head><body><input></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_input_tag_023() 
{
#define  TEST_STRING "<html><head></head><body><input size=\"\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_input_tag_024() 
{
#define  TEST_STRING "<html><head></head><body><input size=\"a\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input size=\"a\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_input_tag_025() 
{
#define  TEST_STRING "<html><head></head><body><input size=\"1\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input size=\"1\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_input_tag_026() 
{
#define  TEST_STRING "<html><head></head><body><input maxlength></body></html>"
#define  RESULT_STRING "<html><head></head><body><input></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_input_tag_027() 
{
#define  TEST_STRING "<html><head></head><body><input maxlength=\"\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_input_tag_028() 
{
#define  TEST_STRING "<html><head></head><body><input maxlength=\"a\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_input_tag_029() 
{
#define  TEST_STRING "<html><head></head><body><input maxlength=\"1\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input maxlength=\"1\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_input_tag_030() 
{
#define  TEST_STRING "<html><head></head><body><input checked></body></html>"
#define  RESULT_STRING "<html><head></head><body><input checked></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_input_tag_031() 
{
#define  TEST_STRING "<html><head></head><body><input accesskey></body></html>"
#define  RESULT_STRING "<html><head></head><body><input></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_input_tag_032() 
{
#define  TEST_STRING "<html><head></head><body><input accesskey=\"\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_input_tag_033() 
{
#define  TEST_STRING "<html><head></head><body><input accesskey=\"1\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input accesskey=\"1\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_input_tag_034() 
{
#define  TEST_STRING "<html><head></head><body><input istyle></body></html>"
#define  RESULT_STRING "<html><head></head><body><input></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_input_tag_035() 
{
#define  TEST_STRING "<html><head></head><body><input istyle=\"\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_input_tag_036() 
{
#define  TEST_STRING "<html><head></head><body><input istyle=\"0\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_input_tag_037() 
{
#define  TEST_STRING "<html><head></head><body><input istyle=\"1\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input mode=\"hiragana\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_input_tag_038() 
{
#define  TEST_STRING "<html><head></head><body><input istyle=\"2\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input mode=\"hankakukana\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_input_tag_039() 
{
#define  TEST_STRING "<html><head></head><body><input istyle=\"3\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input mode=\"alphabet\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_input_tag_040() 
{
#define  TEST_STRING "<html><head></head><body><input istyle=\"4\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input mode=\"numeric\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_input_tag_041()
{
#define  TEST_STRING "<html><head></head><body><input istyle=\"5\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_input_tag_042()
{
#define  TEST_STRING "<html><head></head><body><input istyle=\"a\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_input_tag_043()
{
#define  TEST_STRING "<html><head></head><body><input istyle=\"1\" maxlength=\"10\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input mode=\"hiragana\" maxlength=\"10\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_input_tag_044()
{
#define  TEST_STRING "<html><head></head><body><input istyle=\"2\" maxlength=\"10\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input mode=\"hankakukana\" maxlength=\"10\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_input_tag_045()
{
#define  TEST_STRING "<html><head></head><body><input istyle=\"3\" maxlength=\"10\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input mode=\"alphabet\" maxlength=\"10\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_input_tag_046()
{
#define  TEST_STRING "<html><head></head><body><input istyle=\"4\" maxlength=\"10\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input mode=\"numeric\" maxlength=\"10\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_input_tag_047()
{
#define  TEST_STRING "<html><head></head><body><input istyle=\"a\" maxlength=\"10\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input maxlength=\"10\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* <LI>                                                                       */
/*============================================================================*/
void test_jhtml_li_tag_001() 
{
#define  TEST_STRING "<li></li>"
#define  RESULT_STRING "<li></li>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_li_tag_002() 
{
#define  TEST_STRING "<li>abc</li>"
#define  RESULT_STRING "<li>abc</li>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_li_tag_003() 
{
#define  TEST_STRING "<li>あいうえお</li>"
#define  RESULT_STRING "<li>あいうえお</li>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_li_tag_004() 
{
#define  TEST_STRING "<li>ﾊﾝｶｸ</li>"
#define  RESULT_STRING "<li>ﾊﾝｶｸ</li>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_li_tag_005() 
{
#define  TEST_STRING "<li type>ﾊﾝｶｸ</li>"
#define  RESULT_STRING "<li>ﾊﾝｶｸ</li>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_li_tag_006() 
{
#define  TEST_STRING "<li type=\"\">ﾊﾝｶｸ</li>"
#define  RESULT_STRING "<li>ﾊﾝｶｸ</li>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_li_tag_007() 
{
#define  TEST_STRING "<li type=\"\">ﾊﾝｶｸ</li>"
#define  RESULT_STRING "<li>ﾊﾝｶｸ</li>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_li_tag_008() 
{
#define  TEST_STRING "<li type=\"1\">ﾊﾝｶｸ</li>"
#define  RESULT_STRING "<li type=\"1\">ﾊﾝｶｸ</li>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_li_tag_009() 
{
#define  TEST_STRING "<li type=\"a\">ﾊﾝｶｸ</li>"
#define  RESULT_STRING "<li type=\"a\">ﾊﾝｶｸ</li>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_li_tag_010() 
{
#define  TEST_STRING "<li type=\"A\">ﾊﾝｶｸ</li>"
#define  RESULT_STRING "<li type=\"A\">ﾊﾝｶｸ</li>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_li_tag_011() 
{
#define  TEST_STRING "<li value>ﾊﾝｶｸ</li>"
#define  RESULT_STRING "<li>ﾊﾝｶｸ</li>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_li_tag_012() 
{
#define  TEST_STRING "<li value=\"\">ﾊﾝｶｸ</li>"
#define  RESULT_STRING "<li>ﾊﾝｶｸ</li>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_li_tag_013() 
{
#define  TEST_STRING "<li value=\"1\">ﾊﾝｶｸ</li>"
#define  RESULT_STRING "<li value=\"1\">ﾊﾝｶｸ</li>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_li_tag_014() 
{
#define  TEST_STRING "<li type=\"disc\">ﾊﾝｶｸ</li>"
#define  RESULT_STRING "<li>ﾊﾝｶｸ</li>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_li_tag_015() 
{
#define  TEST_STRING "<li type=\"circle\">ﾊﾝｶｸ</li>"
#define  RESULT_STRING "<li>ﾊﾝｶｸ</li>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_li_tag_016() 
{
#define  TEST_STRING "<li type=\"square\">ﾊﾝｶｸ</li>"
#define  RESULT_STRING "<li>ﾊﾝｶｸ</li>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* <MENU>                                                                     */
/*============================================================================*/
void test_jhtml_menu_tag_001() 
{
#define  TEST_STRING "<menu></menu>"
#define  RESULT_STRING "<menu></menu>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_menu_tag_002() 
{
#define  TEST_STRING "<menu><li></li><li></li></menu>"
#define  RESULT_STRING "<menu><li></li><li></li></menu>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_menu_tag_003() 
{
#define  TEST_STRING "<menu><li>abc</li><li>def</li></menu>"
#define  RESULT_STRING "<menu><li>abc</li><li>def</li></menu>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_menu_tag_004() 
{
#define  TEST_STRING "<menu><li>あああ</li><li>いいい</li></menu>"
#define  RESULT_STRING "<menu><li>あああ</li><li>いいい</li></menu>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_menu_tag_005() 
{
#define  TEST_STRING "<menu><li>ﾊﾝｶｸ</li><li>ﾊﾝｶｸ</li></menu>"
#define  RESULT_STRING "<menu><li>ﾊﾝｶｸ</li><li>ﾊﾝｶｸ</li></menu>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_menu_tag_006() 
{
#define  TEST_STRING "<menu type=\"disc\"><li>ﾊﾝｶｸ</li><li>ﾊﾝｶｸ</li></menu>"
#define  RESULT_STRING "<menu><li>ﾊﾝｶｸ</li><li>ﾊﾝｶｸ</li></menu>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* <OL>                                                                       */
/*============================================================================*/
void test_jhtml_ol_tag_001() 
{
#define  TEST_STRING "<ol></ol>"
#define  RESULT_STRING "<ol></ol>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_ol_tag_002() 
{
#define  TEST_STRING "<ol><li></li></ol>"
#define  RESULT_STRING "<ol><li></li></ol>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_ol_tag_003() 
{
#define  TEST_STRING "<ol><li>abc</li></ol>"
#define  RESULT_STRING "<ol><li>abc</li></ol>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_ol_tag_004() 
{
#define  TEST_STRING "<ol><li>abc</li><li>def</li></ol>"
#define  RESULT_STRING "<ol><li>abc</li><li>def</li></ol>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_ol_tag_005() 
{
#define  TEST_STRING "<ol><li>あ</li></ol>"
#define  RESULT_STRING "<ol><li>あ</li></ol>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_ol_tag_006() 
{
#define  TEST_STRING "<ol><li>あ</li><li>い</li></ol>"
#define  RESULT_STRING "<ol><li>あ</li><li>い</li></ol>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_ol_tag_007() 
{
#define  TEST_STRING "<ol><li>ﾊﾝｶｸ</li></ol>"
#define  RESULT_STRING "<ol><li>ﾊﾝｶｸ</li></ol>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_ol_tag_008() 
{
#define  TEST_STRING "<ol><li>ﾊﾝｶｸ</li><li>ｶﾅﾀﾞﾖ</li></ol>"
#define  RESULT_STRING "<ol><li>ﾊﾝｶｸ</li><li>ｶﾅﾀﾞﾖ</li></ol>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_ol_tag_009() 
{
#define  TEST_STRING "<ol type></ol>"
#define  RESULT_STRING "<ol></ol>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_ol_tag_010() 
{
#define  TEST_STRING "<ol type=\"\"></ol>"
#define  RESULT_STRING "<ol></ol>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_ol_tag_011() 
{
#define  TEST_STRING "<ol type=\"1\"></ol>"
#define  RESULT_STRING "<ol type=\"1\"></ol>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_ol_tag_012() 
{
#define  TEST_STRING "<ol type=\"a\"></ol>"
#define  RESULT_STRING "<ol type=\"a\"></ol>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_ol_tag_013() 
{
#define  TEST_STRING "<ol type=\"A\"></ol>"
#define  RESULT_STRING "<ol type=\"A\"></ol>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_ol_tag_014() 
{
#define  TEST_STRING "<ol type=\"b\"></ol>"
#define  RESULT_STRING "<ol></ol>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_ol_tag_015() 
{
#define  TEST_STRING "<ol start></ol>"
#define  RESULT_STRING "<ol></ol>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_ol_tag_016() 
{
#define  TEST_STRING "<ol start=\"\"></ol>"
#define  RESULT_STRING "<ol></ol>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_ol_tag_017() 
{
#define  TEST_STRING "<ol start=\"1\"></ol>"
#define  RESULT_STRING "<ol start=\"1\"></ol>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_ol_tag_018() 
{
#define  TEST_STRING "<ol start=\"a\"></ol>"
#define  RESULT_STRING "<ol start=\"a\"></ol>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* <OPTION>                                                                   */
/*============================================================================*/
void test_jhtml_option_tag_001() 
{
#define  TEST_STRING "<option></option>"
#define  RESULT_STRING "<option>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_option_tag_002() 
{
#define  TEST_STRING "<option value></option>"
#define  RESULT_STRING "<option>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_option_tag_003() 
{
#define  TEST_STRING "<option value=\"\"></option>"
#define  RESULT_STRING "<option>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_option_tag_004() 
{
#define  TEST_STRING "<option value=\"a\"></option>"
#define  RESULT_STRING "<option value=\"a\">"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_option_tag_005() 
{
#define  TEST_STRING "<option value=\"亀\"></option>"
#define  RESULT_STRING "<option value=\"亀\">"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_option_tag_006() 
{
#define  TEST_STRING "<option value=\"ﾊﾝｶｸ\"></option>"
#define  RESULT_STRING "<option value=\"ﾊﾝｶｸ\">"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_option_tag_007() 
{
#define  TEST_STRING "<option selected></option>"
#define  RESULT_STRING "<option selected>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* <P>                                                                        */
/*============================================================================*/
void test_jhtml_p_tag_001() 
{
#define  TEST_STRING "<p></p>"
#define  RESULT_STRING "<p></p>"
  char  *ret;
  char  *tmp;
  device_table       spec;
  chxjconvrule_entry entry;
  cookie_t           cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_p_tag_002() 
{
#define  TEST_STRING "<p>あああ</p>"
#define  RESULT_STRING "<p>あああ</p>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_p_tag_003() 
{
#define  TEST_STRING "<p align>あああ</p>"
#define  RESULT_STRING "<p>あああ</p>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_p_tag_004() 
{
#define  TEST_STRING "<p align=\"\">あああ</p>"
#define  RESULT_STRING "<p>あああ</p>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_p_tag_005() 
{
#define  TEST_STRING "<p align=\"right\">あああ</p>"
#define  RESULT_STRING "<p align=\"right\">あああ</p>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_p_tag_006() 
{
#define  TEST_STRING "<p align=\"left\">あああ</p>"
#define  RESULT_STRING "<p align=\"left\">あああ</p>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_p_tag_007() 
{
#define  TEST_STRING "<p align=\"center\">あああ</p>"
#define  RESULT_STRING "<p align=\"center\">あああ</p>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_p_tag_008() 
{
#define  TEST_STRING "<p align=\"unknown\">あああ</p>"
#define  RESULT_STRING "<p>あああ</p>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* <PLAINTEXT>                                                                */
/*============================================================================*/
void test_jhtml_plaintext_tag_001() 
{
#define  TEST_STRING "<plaintext>あああ</plaintext>"
#define  RESULT_STRING "<plaintext>あああ"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_plaintext_tag_002() 
{
#define  TEST_STRING "<plaintext><h1>あああ</h1><h1>いいい</h1></plaintext>"
#define  RESULT_STRING "<plaintext><h1>あああ</h1><h1>いいい</h1>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_plaintext_tag_003() 
{
#define  TEST_STRING "<plaintext><h1>あああ<h1>いいい</h1></plaintext>"
#define  RESULT_STRING "<plaintext><h1>あああ<h1>いいい</h1>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_plaintext_tag_004() 
{
#define  TEST_STRING "<plaintext><h1>ﾊﾝｶｸ<h1>ﾊﾝｶｸ</plaintext>"
#define  RESULT_STRING "<plaintext><h1>ﾊﾝｶｸ<h1>ﾊﾝｶｸ"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* <PRE>                                                                      */
/*============================================================================*/
void test_jhtml_pre_tag_001() 
{
#define  TEST_STRING "<pre></pre>"
#define  RESULT_STRING "<pre></pre>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_pre_tag_002() 
{
#define  TEST_STRING "<pre>\n</pre>"
#define  RESULT_STRING "<pre>\n</pre>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_pre_tag_003() 
{
#define  TEST_STRING "<pre>\n\n</pre>"
#define  RESULT_STRING "<pre>\n\n</pre>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_pre_tag_004() 
{
#define  TEST_STRING "<pre><h1>あああ\n\n</pre>"
#define  RESULT_STRING "<pre>あああ\n\n</pre>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_pre_tag_005() 
{
#define  TEST_STRING "<pre><h1>ﾊﾝｶｸ</h1>\n\n</pre>"
#define  RESULT_STRING "<pre>ﾊﾝｶｸ\n\n</pre>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* <SELECT>                                                                   */
/*============================================================================*/
void test_jhtml_select_tag_001() 
{
#define  TEST_STRING "<select></select>"
#define  RESULT_STRING "<select></select>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_select_tag_002() 
{
#define  TEST_STRING "<select>あああ</select>"
#define  RESULT_STRING "<select>あああ</select>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_select_tag_003() 
{
#define  TEST_STRING "<select name></select>"
#define  RESULT_STRING "<select></select>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_select_tag_004() 
{
#define  TEST_STRING "<select name=\"\"></select>"
#define  RESULT_STRING "<select></select>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_select_tag_005() 
{
#define  TEST_STRING "<select name=\"abc\"></select>"
#define  RESULT_STRING "<select name=\"abc\"></select>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_select_tag_006() 
{
#define  TEST_STRING "<select name=\"あああ\"></select>"
#define  RESULT_STRING "<select name=\"あああ\"></select>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_select_tag_007() 
{
#define  TEST_STRING "<select name=\"ﾊﾝｶｸ\"></select>"
#define  RESULT_STRING "<select name=\"ﾊﾝｶｸ\"></select>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_select_tag_008() 
{
#define  TEST_STRING "<select size></select>"
#define  RESULT_STRING "<select></select>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_select_tag_009() 
{
#define  TEST_STRING "<select size=\"\"></select>"
#define  RESULT_STRING "<select></select>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_select_tag_010() 
{
#define  TEST_STRING "<select size=\"abc\"></select>"
#define  RESULT_STRING "<select size=\"abc\"></select>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_select_tag_011() 
{
#define  TEST_STRING "<select size=\"11\"></select>"
#define  RESULT_STRING "<select size=\"11\"></select>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_select_tag_012() 
{
#define  TEST_STRING "<select multiple></select>"
#define  RESULT_STRING "<select multiple></select>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_select_tag_013() 
{
#define  TEST_STRING "<select multiple=\"\"></select>"
#define  RESULT_STRING "<select multiple></select>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_select_tag_014() 
{
#define  TEST_STRING "<select multiple=\"abc\"></select>"
#define  RESULT_STRING "<select multiple></select>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* <TEXTAREA>                                                                 */
/*============================================================================*/
void test_jhtml_textarea_tag_001() 
{
#define  TEST_STRING "<textarea>"
#define  RESULT_STRING "<textarea></textarea>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_textarea_tag_002() 
{
#define  TEST_STRING "<textarea></textarea>"
#define  RESULT_STRING "<textarea></textarea>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_textarea_tag_003() 
{
#define  TEST_STRING "<textarea>あああ</textarea>"
#define  RESULT_STRING "<textarea>あああ</textarea>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_textarea_tag_004() 
{
#define  TEST_STRING "<textarea>あああ\nいいい</textarea>"
#define  RESULT_STRING "<textarea>あああ\nいいい</textarea>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_textarea_tag_005() 
{
#define  TEST_STRING "<textarea>あああ\r\nいいい</textarea>"
#define  RESULT_STRING "<textarea>あああ\r\nいいい</textarea>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_textarea_tag_006() 
{
#define  TEST_STRING "<textarea>あああ\r\nいいい\n</textarea>"
#define  RESULT_STRING "<textarea>あああ\r\nいいい\n</textarea>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_textarea_tag_007() 
{
#define  TEST_STRING "<textarea>あああ\r\nいいい\r\n</textarea>"
#define  RESULT_STRING "<textarea>あああ\r\nいいい\r\n</textarea>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_textarea_tag_008() 
{
#define  TEST_STRING "<textarea name></textarea>"
#define  RESULT_STRING "<textarea></textarea>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_textarea_tag_009() 
{
#define  TEST_STRING "<textarea name=\"\"></textarea>"
#define  RESULT_STRING "<textarea></textarea>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_textarea_tag_010() 
{
#define  TEST_STRING "<textarea name=\"aaa\"></textarea>"
#define  RESULT_STRING "<textarea name=\"aaa\"></textarea>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_textarea_tag_011() 
{
#define  TEST_STRING "<textarea rows></textarea>"
#define  RESULT_STRING "<textarea></textarea>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_textarea_tag_012() 
{
#define  TEST_STRING "<textarea rows=\"\"></textarea>"
#define  RESULT_STRING "<textarea></textarea>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_textarea_tag_013() 
{
#define  TEST_STRING "<textarea rows=\"abc\"></textarea>"
#define  RESULT_STRING "<textarea rows=\"abc\"></textarea>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_textarea_tag_014() 
{
#define  TEST_STRING "<textarea cols></textarea>"
#define  RESULT_STRING "<textarea></textarea>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_textarea_tag_015() 
{
#define  TEST_STRING "<textarea cols=\"\"></textarea>"
#define  RESULT_STRING "<textarea></textarea>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_textarea_tag_016() 
{
#define  TEST_STRING "<textarea cols=\"123\"></textarea>"
#define  RESULT_STRING "<textarea cols=\"123\"></textarea>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_textarea_tag_017() 
{
#define  TEST_STRING "<textarea accesskey></textarea>"
#define  RESULT_STRING "<textarea></textarea>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_textarea_tag_018() 
{
#define  TEST_STRING "<textarea accesskey=\"\"></textarea>"
#define  RESULT_STRING "<textarea></textarea>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_textarea_tag_019() 
{
#define  TEST_STRING "<textarea accesskey=\"10\"></textarea>"
#define  RESULT_STRING "<textarea accesskey=\"10\"></textarea>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_textarea_tag_020() 
{
#define  TEST_STRING "<textarea istyle></textarea>"
#define  RESULT_STRING "<textarea></textarea>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_textarea_tag_021() 
{
#define  TEST_STRING "<textarea istyle=\"\"></textarea>"
#define  RESULT_STRING "<textarea></textarea>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_textarea_tag_022() 
{
#define  TEST_STRING "<textarea istyle=\"1\"></textarea>"
#define  RESULT_STRING "<textarea mode=\"hiragana\"></textarea>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_textarea_tag_023() 
{
#define  TEST_STRING "<textarea istyle=\"2\"></textarea>"
#define  RESULT_STRING "<textarea mode=\"hankakukana\"></textarea>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_textarea_tag_024() 
{
#define  TEST_STRING "<textarea istyle=\"3\"></textarea>"
#define  RESULT_STRING "<textarea mode=\"alphabet\"></textarea>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_textarea_tag_025() 
{
#define  TEST_STRING "<textarea istyle=\"4\"></textarea>"
#define  RESULT_STRING "<textarea mode=\"numeric\"></textarea>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_textarea_tag_026() 
{
#define  TEST_STRING "<textarea istyle=\"a\"></textarea>"
#define  RESULT_STRING "<textarea></textarea>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* <TITLE>                                                                    */
/*============================================================================*/
void test_jhtml_title_tag_001()
{
#define  TEST_STRING "<title></title>"
#define  RESULT_STRING "<title></title>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_title_tag_002()
{
#define  TEST_STRING "<title>あああ</title>"
#define  RESULT_STRING "<title>あああ</title>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* <UL>                                                                       */
/*============================================================================*/
void test_jhtml_ul_tag_001() 
{
#define  TEST_STRING "<ul>"
#define  RESULT_STRING "<ul></ul>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_ul_tag_002() 
{
#define  TEST_STRING "<ul></ul>"
#define  RESULT_STRING "<ul></ul>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_ul_tag_003() 
{
#define  TEST_STRING "<ul><li></ul>"
#define  RESULT_STRING "<ul><li></li></ul>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_ul_tag_004() 
{
#define  TEST_STRING "<ul abc><li></ul>"
#define  RESULT_STRING "<ul><li></li></ul>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_ul_tag_005() 
{
#define  TEST_STRING "<ul abc><li>abc</ul>"
#define  RESULT_STRING "<ul><li>abc</li></ul>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_ul_tag_006() 
{
#define  TEST_STRING "<ul type=\"disc\"><li>abc</ul>"
#define  RESULT_STRING "<ul><li>abc</li></ul>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* <BLINK>                                                                    */
/*============================================================================*/
void test_jhtml_blink_tag_001() 
{
#define  TEST_STRING "<blink></blink>"
#define  RESULT_STRING "<blink></blink>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_blink_tag_002() 
{
#define  TEST_STRING "<blink>abc</blink>"
#define  RESULT_STRING "<blink>abc</blink>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_blink_tag_003() 
{
#define  TEST_STRING "<blink>あああ</blink>"
#define  RESULT_STRING "<blink>あああ</blink>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_blink_tag_004() 
{
#define  TEST_STRING "<blink>ﾊﾝｶｸ</blink>"
#define  RESULT_STRING "<blink>ﾊﾝｶｸ</blink>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* MARQUEE                                                                    */
/*============================================================================*/
void test_jhtml_marquee_tag_001() 
{
#define  TEST_STRING "<marquee></marquee>"
#define  RESULT_STRING "<marquee></marquee>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_marquee_tag_002() 
{
#define  TEST_STRING "<marquee>abc</marquee>"
#define  RESULT_STRING "<marquee>abc</marquee>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_marquee_tag_003() 
{
#define  TEST_STRING "<marquee>あああ</marquee>"
#define  RESULT_STRING "<marquee>あああ</marquee>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_marquee_tag_004() 
{
#define  TEST_STRING "<marquee>ﾊﾝｶｸ</marquee>"
#define  RESULT_STRING "<marquee>ﾊﾝｶｸ</marquee>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_marquee_tag_005() 
{
#define  TEST_STRING "<marquee direction>ﾊﾝｶｸ</marquee>"
#define  RESULT_STRING "<marquee>ﾊﾝｶｸ</marquee>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_marquee_tag_006() 
{
#define  TEST_STRING "<marquee direction=\"\">ﾊﾝｶｸ</marquee>"
#define  RESULT_STRING "<marquee>ﾊﾝｶｸ</marquee>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_marquee_tag_007() 
{
#define  TEST_STRING "<marquee direction=\"left\">ﾊﾝｶｸ</marquee>"
#define  RESULT_STRING "<marquee direction=\"left\">ﾊﾝｶｸ</marquee>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_marquee_tag_008() 
{
#define  TEST_STRING "<marquee direction=\"right\">ﾊﾝｶｸ</marquee>"
#define  RESULT_STRING "<marquee direction=\"right\">ﾊﾝｶｸ</marquee>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_marquee_tag_009() 
{
#define  TEST_STRING "<marquee direction=\"center\">ﾊﾝｶｸ</marquee>"
#define  RESULT_STRING "<marquee>ﾊﾝｶｸ</marquee>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_marquee_tag_010() 
{
#define  TEST_STRING "<marquee behavior>ﾊﾝｶｸ</marquee>"
#define  RESULT_STRING "<marquee>ﾊﾝｶｸ</marquee>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_marquee_tag_011() 
{
#define  TEST_STRING "<marquee behavior=\"\">ﾊﾝｶｸ</marquee>"
#define  RESULT_STRING "<marquee>ﾊﾝｶｸ</marquee>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_marquee_tag_012() 
{
#define  TEST_STRING "<marquee behavior=\"scroll\">ﾊﾝｶｸ</marquee>"
#define  RESULT_STRING "<marquee>ﾊﾝｶｸ</marquee>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_marquee_tag_013() 
{
#define  TEST_STRING "<marquee behavior=\"slide\">ﾊﾝｶｸ</marquee>"
#define  RESULT_STRING "<marquee>ﾊﾝｶｸ</marquee>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_marquee_tag_014() 
{
#define  TEST_STRING "<marquee behavior=\"alternate\">ﾊﾝｶｸ</marquee>"
#define  RESULT_STRING "<marquee>ﾊﾝｶｸ</marquee>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_marquee_tag_015() 
{
#define  TEST_STRING "<marquee behavior=\"unknown\">ﾊﾝｶｸ</marquee>"
#define  RESULT_STRING "<marquee>ﾊﾝｶｸ</marquee>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_marquee_tag_016() 
{
#define  TEST_STRING "<marquee loop>ﾊﾝｶｸ</marquee>"
#define  RESULT_STRING "<marquee>ﾊﾝｶｸ</marquee>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_marquee_tag_017() 
{
#define  TEST_STRING "<marquee loop=\"\">ﾊﾝｶｸ</marquee>"
#define  RESULT_STRING "<marquee>ﾊﾝｶｸ</marquee>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_marquee_tag_018() 
{
#define  TEST_STRING "<marquee loop=\"1\">ﾊﾝｶｸ</marquee>"
#define  RESULT_STRING "<marquee>ﾊﾝｶｸ</marquee>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_marquee_tag_019() 
{
#define  TEST_STRING "<marquee bgcolor=\"#ff0000\">ﾊﾝｶｸ</marquee>"
#define  RESULT_STRING "<marquee>ﾊﾝｶｸ</marquee>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_marquee_tag_020() 
{
#define  TEST_STRING "<marquee bgcolor=\"\">ﾊﾝｶｸ</marquee>"
#define  RESULT_STRING "<marquee>ﾊﾝｶｸ</marquee>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_marquee_tag_021() 
{
#define  TEST_STRING "<marquee bgcolor>ﾊﾝｶｸ</marquee>"
#define  RESULT_STRING "<marquee>ﾊﾝｶｸ</marquee>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/* KONNO */
/*============================================================================*/
/* <META>                                                                     */
/*============================================================================*/
void test_jhtml_meta_tag_001() 
{
#define  TEST_STRING "<meta>"
#define  RESULT_STRING "<meta>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_meta_tag_002() 
{
#define  TEST_STRING "<meta http-equiv>"
#define  RESULT_STRING "<meta>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_meta_tag_003() 
{
#define  TEST_STRING "<meta http-equiv=\"\">"
#define  RESULT_STRING "<meta>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_meta_tag_004() 
{
#define  TEST_STRING "<meta http-equiv=\"content-type\">"
#define  RESULT_STRING "<meta http-equiv=\"content-type\">"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_meta_tag_005() 
{
#define  TEST_STRING "<meta http-equiv=\"abc\">"
#define  RESULT_STRING "<meta http-equiv=\"abc\">"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_meta_tag_006() 
{
#define  TEST_STRING "<meta content>"
#define  RESULT_STRING "<meta>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_meta_tag_007() 
{
#define  TEST_STRING "<meta content=\"\">"
#define  RESULT_STRING "<meta>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_meta_tag_008() 
{
#define  TEST_STRING "<meta http-equiv=\"Content-Type\" content=\"text/html\">"
#define  RESULT_STRING "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=Windows-31J\">"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_meta_tag_009() 
{
#define  TEST_STRING "<meta http-equiv=\"Content-Type\" content=\"application/xhtml+xml\">"
#define  RESULT_STRING "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=Windows-31J\">"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* <FONT>                                                                     */
/*============================================================================*/
void test_jhtml_font_tag_001() 
{
#define  TEST_STRING "<font>"
#define  RESULT_STRING "\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_font_tag_002() 
{
#define  TEST_STRING "<font>あああ</font>"
#define  RESULT_STRING "あああ"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_font_tag_003() 
{
#define  TEST_STRING "<font>ｱｱｱ</font>"
#define  RESULT_STRING "ｱｱｱ"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_font_tag_004() 
{
#define  TEST_STRING "<font color>ｱｱｱ</font>"
#define  RESULT_STRING "ｱｱｱ"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_font_tag_005() 
{
#define  TEST_STRING "<font color=\"\">ｱｱｱ</font>"
#define  RESULT_STRING "ｱｱｱ"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_font_tag_006() 
{
#define  TEST_STRING "<font color=\"#ff0000\">ｱｱｱ</font>"
#define  RESULT_STRING "<font color=\"#ff0000\">ｱｱｱ</font>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_font_tag_007() 
{
#define  TEST_STRING "<font size=\"1\">ｱｱｱ</font>"
#define  RESULT_STRING "ｱｱｱ"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_font_tag_008() 
{
#define  TEST_STRING "<font size=\"\">ｱｱｱ</font>"
#define  RESULT_STRING "ｱｱｱ"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_font_tag_009() 
{
#define  TEST_STRING "<font size>ｱｱｱ</font>"
#define  RESULT_STRING "ｱｱｱ"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_font_tag_010() 
{
#define  TEST_STRING "<font size=\"+1\">ｱｱｱ</font>"
#define  RESULT_STRING "ｱｱｱ"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*===========================================================================*/
/* param                                                                     */
/*===========================================================================*/
void test_jhtml_param_tag_001() 
{
#define  TEST_STRING "<param>aaa</param>"
#define  RESULT_STRING "\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}


/* CSS */
static int call_check = 0;
char *test_chxj_serf_get001(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_link_001() 
{
#define  TEST_STRING "<link rel=\"stylesheet\" href=\"http://localhost/a.css\" type=\"text/css\" />"
#define  RESULT_STRING "\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get001;

  call_check = 0;
  COOKIE_INIT(cookie);
  entry.action |= CONVRULE_CSS_ON_BIT;

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_link_002() 
{
#define  TEST_STRING "<link rel=\"abc\" href=\"http://localhost/a.css\" type=\"text/css\" />"
#define  RESULT_STRING "\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get001;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_link_003() 
{
#define  TEST_STRING "<link rel=\"\" href=\"http://localhost/a.css\" type=\"text/css\" />"
#define  RESULT_STRING "\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get001;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_link_004() 
{
#define  TEST_STRING "<link rel href=\"http://localhost/a.css\" type=\"text/css\" />"
#define  RESULT_STRING "\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get001;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_link_005() 
{
#define  TEST_STRING "<link rel=\"stylesheet\" href=\"\" type=\"text/css\" />"
#define  RESULT_STRING "\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get001;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_link_006() 
{
#define  TEST_STRING "<link rel=\"stylesheet\" href type=\"text/css\" />"
#define  RESULT_STRING "\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get001;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_link_007() 
{
#define  TEST_STRING "<link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"\" />"
#define  RESULT_STRING "\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get001;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_link_008() 
{
#define  TEST_STRING "<link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type />"
#define  RESULT_STRING "\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get001;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_link_009() 
{
#define  TEST_STRING "<link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/abc\" />"
#define  RESULT_STRING "\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get001;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}



/*===========================================================================*/
/* HTML tag with CSS                                                         */
/*===========================================================================*/
char *test_chxj_serf_get002(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "html    { display: none }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_html_tag_with_css_001()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" /></head><body></body></html>"
#define  RESULT_STRING "<html><head></head><body></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get002;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}

/******************************************************************************/
/* meta with CSS                                                              */
/******************************************************************************/
char *test_chxj_serf_get003(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "meta    { display: none }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_meta_tag_with_css_001()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "<META http-equiv=\"Content-Type\" content=\"text/html; charset=SHIFT_JIS\"></head><body></body></html>"
#define  RESULT_STRING "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=Windows-31J\"></head><body></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get003;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}



/******************************************************************************/
/* TEXTAREA with CSS                                                          */
/******************************************************************************/
char *test_chxj_serf_get004(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "textarea { -wap-input-format: &quot;*&lt;ja:h&gt;&quot; }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_textarea_tag_with_css_001()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><textarea></textarea></body></html>"
#define  RESULT_STRING "<html><head></head><body><textarea mode=\"hiragana\"></textarea></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get004;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get005(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "textarea { -wap-input-format: &quot;*&lt;ja:hk&gt;&quot; }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_textarea_tag_with_css_002()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><textarea></textarea></body></html>"
#define  RESULT_STRING "<html><head></head><body><textarea mode=\"hankakukana\"></textarea></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get005;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  fprintf(stderr, "actual:[%s]\n", ret);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}

char *test_chxj_serf_get006(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "textarea { -wap-input-format: &quot;*&lt;ja:en&gt;&quot; }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_textarea_tag_with_css_003()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><textarea></textarea></body></html>"
#define  RESULT_STRING "<html><head></head><body><textarea mode=\"alphabet\"></textarea></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get006;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}

char *test_chxj_serf_get007(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "textarea { -wap-input-format: &quot;*&lt;ja:n&gt;&quot;; }\n"
                     "br      { display: none }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_textarea_tag_with_css_004()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><textarea></textarea></body></html>"
#define  RESULT_STRING "<html><head></head><body><textarea mode=\"numeric\"></textarea></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get007;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}


/*===========================================================================*/
/* TEXTAREA with style="-wap-input-format"                                   */
/*===========================================================================*/
void test_jhtml_textarea_tag_with_css_005()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><textarea style=\"-wap-input-format: &quot;*&lt;ja:h&gt;&quot;\"></textarea></body></html>"
#define  RESULT_STRING "<html><head></head><body><textarea mode=\"hiragana\"></textarea></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get007;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_textarea_tag_with_css_006()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><textarea style=\"-wap-input-format: &quot;*&lt;ja:hk&gt;&quot;\"></textarea></body></html>"
#define  RESULT_STRING "<html><head></head><body><textarea mode=\"hankakukana\"></textarea></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get007;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_textarea_tag_with_css_007()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><textarea style=\"-wap-input-format: &quot;*&lt;ja:en&gt;&quot;\"></textarea></body></html>"
#define  RESULT_STRING "<html><head></head><body><textarea mode=\"alphabet\"></textarea></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get007;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_textarea_tag_with_css_008()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><textarea style=\"-wap-input-format: &quot;*&lt;ja:n&gt;&quot;\"></textarea></body></html>"
#define  RESULT_STRING "<html><head></head><body><textarea mode=\"numeric\"></textarea></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get007;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}



/*===========================================================================*/
/* p with CSS                                                                */
/*===========================================================================*/
char *test_chxj_serf_get008(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "p       { text-align: left }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_p_tag_with_css_001()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><p>あいう</p></body></html>"
#define  RESULT_STRING "<html><head></head><body><p align=\"left\">あいう</p></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get008;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get009(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "p       { text-align: center }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_p_tag_with_css_002()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><p>あいう</p></body></html>"
#define  RESULT_STRING "<html><head></head><body><p align=\"center\">あいう</p></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get009;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get010(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "p       { text-align: right }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_p_tag_with_css_003()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><p>あいう</p></body></html>"
#define  RESULT_STRING "<html><head></head><body><p align=\"right\">あいう</p></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get010;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}

void test_jhtml_p_tag_with_css_004()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><p style=\"text-align: left\">あいう</p></body></html>"
#define  RESULT_STRING "<html><head></head><body><p align=\"left\">あいう</p></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get010;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_p_tag_with_css_005()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><p style=\"text-align: center\">あいう</p></body></html>"
#define  RESULT_STRING "<html><head></head><body><p align=\"center\">あいう</p></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get010;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_p_tag_with_css_006()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><p style=\"text-align: right\">あいう</p></body></html>"
#define  RESULT_STRING "<html><head></head><body><p align=\"right\">あいう</p></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get010;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_p_tag_with_css_007()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><p style=\"text-align: right\"><p>あいう</p></p></body></html>"
#define  RESULT_STRING "<html><head></head><body><p align=\"right\"><p align=\"right\">あいう</p></p></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get010;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*=======================*/
/* p with color property */
/*=======================*/
char *test_chxj_serf_get011(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "p       { color: #ff0000 }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_p_tag_with_css_008()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><p>あいう</p></body></html>"
#define  RESULT_STRING "<html><head></head><body><p><font color=\"#ff0000\">あいう</font></p></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get011;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get012(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "p       { color: rgb(100%, 100%, 100%) }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_p_tag_with_css_009()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><p>あいう</p></body></html>"
#define  RESULT_STRING "<html><head></head><body><p><font color=\"#ffffff\">あいう</font></p></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get012;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_p_tag_with_css_010()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><p style=\"color: #ff0000\">あいう</p></body></html>"
#define  RESULT_STRING "<html><head></head><body><p><font color=\"#ff0000\">あいう</font></p></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get011;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_p_tag_with_css_011()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><p style=\"color: rgb(100%, 100%, 100%)\">あいう</p></body></html>"
#define  RESULT_STRING "<html><head></head><body><p><font color=\"#ffffff\">あいう</font></p></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get012;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get013(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "p       { text-decoration: blink }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_p_tag_with_css_012()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><p>あいう</p></body></html>"
#define  RESULT_STRING "<html><head></head><body><p><blink>あいう</blink></p></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get013;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_p_tag_with_css_013()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><p style=\"text-decoration: blink\">あいう</p></body></html>"
#define  RESULT_STRING "<html><head></head><body><p><blink>あいう</blink></p></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get013;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}


/*===========================================================================*/
/* li tag with CSS                                                           */
/*===========================================================================*/
char *test_chxj_serf_get017(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "li      { list-style-type: decimal }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_li_tag_with_css_001()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><ul><li>あいう</li><li>かきく</li></ul></body></html>"
#define  RESULT_STRING "<html><head></head><body><ul><li type=\"1\">あいう</li><li type=\"1\">かきく</li></ul></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get017;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get018(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "li      { list-style-type: upper-alpha }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_li_tag_with_css_002()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><ul><li>あいう</li><li>かきく</li></ul></body></html>"
#define  RESULT_STRING "<html><head></head><body><ul><li type=\"A\">あいう</li><li type=\"A\">かきく</li></ul></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get018;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get019(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "li      { list-style-type: lower-alpha }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_li_tag_with_css_003()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><ul><li>あいう</li><li>かきく</li></ul></body></html>"
#define  RESULT_STRING "<html><head></head><body><ul><li type=\"a\">あいう</li><li type=\"a\">かきく</li></ul></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get019;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get020(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "li      { list-style-type: disc }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_li_tag_with_css_004()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><ul><li>あいう</li><li>かきく</li></ul></body></html>"
#define  RESULT_STRING "<html><head></head><body><ul><li>あいう</li><li>かきく</li></ul></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get020;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get021(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "li      { list-style-type: circle }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_li_tag_with_css_005()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><ul><li>あいう</li><li>かきく</li></ul></body></html>"
#define  RESULT_STRING "<html><head></head><body><ul><li>あいう</li><li>かきく</li></ul></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get021;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get022(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "li      { list-style-type: square }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_li_tag_with_css_006()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><ul><li>あいう</li><li>かきく</li></ul></body></html>"
#define  RESULT_STRING "<html><head></head><body><ul><li>あいう</li><li>かきく</li></ul></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get022;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_li_tag_with_css_007()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><ul><li style=\"list-style-type: decimal\">あいう</li><li>かきく</li></ul></body></html>"
#define  RESULT_STRING "<html><head></head><body><ul><li type=\"1\">あいう</li><li>かきく</li></ul></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get022;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_li_tag_with_css_008()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><ul><li style=\"list-style-type: upper-alpha\">あいう</li><li>かきく</li></ul></body></html>"
#define  RESULT_STRING "<html><head></head><body><ul><li type=\"A\">あいう</li><li>かきく</li></ul></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get022;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_li_tag_with_css_009()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><ul><li style=\"list-style-type: lower-alpha\">あいう</li><li>かきく</li></ul></body></html>"
#define  RESULT_STRING "<html><head></head><body><ul><li type=\"a\">あいう</li><li>かきく</li></ul></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get022;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_li_tag_with_css_010()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><ul><li style=\"list-style-type: disc\">あいう</li><li>かきく</li></ul></body></html>"
#define  RESULT_STRING "<html><head></head><body><ul><li>あいう</li><li>かきく</li></ul></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get022;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_li_tag_with_css_011()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><ul><li style=\"list-style-type: circle\">あいう</li><li>かきく</li></ul></body></html>"
#define  RESULT_STRING "<html><head></head><body><ul><li>あいう</li><li>かきく</li></ul></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get022;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_li_tag_with_css_012()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><ul><li style=\"list-style-type: square\">あいう</li><li>かきく</li></ul></body></html>"
#define  RESULT_STRING "<html><head></head><body><ul><li>あいう</li><li>かきく</li></ul></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get022;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}


/*===========================================================================*/
/* ol tag with CSS                                                           */
/*===========================================================================*/
char *test_chxj_serf_get030(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "ol      { list-style-type: decimal }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_ol_tag_with_css_001()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><ol><li>あいう</li><li>かきく</li></ul></body></html>"
#define  RESULT_STRING "<html><head></head><body><ol type=\"1\"><li type=\"1\">あいう</li><li type=\"1\">かきく</li></ol></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get030;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get031(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "ol      { list-style-type: lower-alpha }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_ol_tag_with_css_002()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><ol><li>あいう</li><li>かきく</li></ul></body></html>"
#define  RESULT_STRING "<html><head></head><body><ol type=\"a\"><li type=\"a\">あいう</li><li type=\"a\">かきく</li></ol></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get031;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get032(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "ol      { list-style-type: upper-alpha }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_ol_tag_with_css_003()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><ol><li>あいう</li><li>かきく</li></ul></body></html>"
#define  RESULT_STRING "<html><head></head><body><ol type=\"A\"><li type=\"A\">あいう</li><li type=\"A\">かきく</li></ol></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get032;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_ol_tag_with_css_004()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><ol style=\"list-style-type:decimal\"><li>あいう</li><li>かきく</li></ul></body></html>"
#define  RESULT_STRING "<html><head></head><body><ol type=\"1\"><li type=\"1\">あいう</li><li type=\"1\">かきく</li></ol></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get032;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_ol_tag_with_css_005()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><ol style=\"list-style-type:lower-alpha\"><li>あいう</li><li>かきく</li></ul></body></html>"
#define  RESULT_STRING "<html><head></head><body><ol type=\"a\"><li type=\"a\">あいう</li><li type=\"a\">かきく</li></ol></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get032;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_ol_tag_with_css_006()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><ol style=\"list-style-type:upper-alpha\"><li>あいう</li><li>かきく</li></ul></body></html>"
#define  RESULT_STRING "<html><head></head><body><ol type=\"A\"><li type=\"A\">あいう</li><li type=\"A\">かきく</li></ol></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get032;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}



/*===========================================================================*/
/* h1 tag with CSS                                                           */
/*===========================================================================*/
char *test_chxj_serf_get040(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "h1      { text-align: right }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_h1_tag_with_css_001()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><h1>あいう</h1></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"right\">あいう</div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get040;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get041(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "h1      { text-align: center }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_h1_tag_with_css_002()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><h1>あいう</h1></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"center\">あいう</div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get041;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get042(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "h1      { text-align: right }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_h1_tag_with_css_003()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><h1>あいう</h1></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"right\">あいう</div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get042;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h1_tag_with_css_004()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><h1 style=\"text-align: left\">あいう</h1></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"left\">あいう</div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get042;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h1_tag_with_css_005()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><h1 style=\"text-align: center\">あいう</h1></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"center\">あいう</div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get042;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h1_tag_with_css_006()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><h1 style=\"text-align: right\">あいう</h1></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"right\">あいう</div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get042;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}


/*===========================================================================*/
/* h2 tag with CSS                                                           */
/*===========================================================================*/
char *test_chxj_serf_get043(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "h2      { text-align: right }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_h2_tag_with_css_001()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><h2>あいう</h2></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"right\">あいう</div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get043;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get044(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "h2      { text-align: center }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_h2_tag_with_css_002()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><h2>あいう</h2></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"center\">あいう</div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get044;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get045(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "h2      { text-align: right }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_h2_tag_with_css_003()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><h2>あいう</h2></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"right\">あいう</div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get045;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h2_tag_with_css_004()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><h2 style=\"text-align: left\">あいう</h2></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"left\">あいう</div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get042;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h2_tag_with_css_005()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><h2 style=\"text-align: center\">あいう</h2></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"center\">あいう</div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get042;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h2_tag_with_css_006()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><h2 style=\"text-align: right\">あいう</h2></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"right\">あいう</div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get042;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}



/*===========================================================================*/
/* h3 tag with CSS                                                           */
/*===========================================================================*/
char *test_chxj_serf_get046(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "h3      { text-align: right }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_h3_tag_with_css_001()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><h3>あいう</h3></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"right\">あいう</div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get046;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get047(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "h3      { text-align: center }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_h3_tag_with_css_002()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><h3>あいう</h3></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"center\">あいう</div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get047;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get048(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "h3      { text-align: right }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_h3_tag_with_css_003()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><h3>あいう</h3></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"right\">あいう</div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get048;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h3_tag_with_css_004()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><h3 style=\"text-align: left\">あいう</h3></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"left\">あいう</div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get048;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h3_tag_with_css_005()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><h3 style=\"text-align: center\">あいう</h3></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"center\">あいう</div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get048;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h3_tag_with_css_006()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><h3 style=\"text-align: right\">あいう</h3></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"right\">あいう</div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get048;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}



/*===========================================================================*/
/* h4 tag with CSS                                                           */
/*===========================================================================*/
char *test_chxj_serf_get049(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "h4      { text-align: right }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_h4_tag_with_css_001()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><h4>あいう</h4></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"right\">あいう</div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get049;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get050(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "h4      { text-align: center }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_h4_tag_with_css_002()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><h4>あいう</h4></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"center\">あいう</div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get050;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get051(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "h4      { text-align: right }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_h4_tag_with_css_003()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><h4>あいう</h4></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"right\">あいう</div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get051;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h4_tag_with_css_004()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><h4 style=\"text-align: left\">あいう</h4></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"left\">あいう</div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get051;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h4_tag_with_css_005()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><h4 style=\"text-align: center\">あいう</h4></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"center\">あいう</div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get051;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h4_tag_with_css_006()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><h4 style=\"text-align: right\">あいう</h4></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"right\">あいう</div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get051;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}



/*===========================================================================*/
/* h5 tag with CSS                                                           */
/*===========================================================================*/
char *test_chxj_serf_get052(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "h5      { text-align: right }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_h5_tag_with_css_001()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><h5>あいう</h5></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"right\">あいう</div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get052;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get053(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "h5      { text-align: center }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_h5_tag_with_css_002()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><h5>あいう</h5></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"center\">あいう</div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get053;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get054(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "h5      { text-align: right }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_h5_tag_with_css_003()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><h5>あいう</h5></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"right\">あいう</div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get054;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h5_tag_with_css_004()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><h5 style=\"text-align: left\">あいう</h5></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"left\">あいう</div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get054;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h5_tag_with_css_005()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><h5 style=\"text-align: center\">あいう</h5></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"center\">あいう</div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get054;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h5_tag_with_css_006()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><h5 style=\"text-align: right\">あいう</h5></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"right\">あいう</div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get054;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}



/*===========================================================================*/
/* h6 tag with CSS                                                           */
/*===========================================================================*/
char *test_chxj_serf_get055(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "h6      { text-align: right }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_h6_tag_with_css_001()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><h6>あいう</h6></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"right\">あいう</div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get055;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get056(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "h6      { text-align: center }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_h6_tag_with_css_002()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><h6>あいう</h6></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"center\">あいう</div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get056;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get057(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "h6      { text-align: right }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_h6_tag_with_css_003()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><h6>あいう</h6></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"right\">あいう</div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get057;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h6_tag_with_css_004()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><h6 style=\"text-align: left\">あいう</h6></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"left\">あいう</div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get057;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h6_tag_with_css_005()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><h6 style=\"text-align: center\">あいう</h6></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"center\">あいう</div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get057;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_h6_tag_with_css_006()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><h6 style=\"text-align: right\">あいう</h6></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"right\">あいう</div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get057;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}




/*===========================================================================*/
/* body tag with CSS                                                         */
/*===========================================================================*/
char *test_chxj_serf_get058(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "body    { color:#ff0000 }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_body_tag_with_css_001()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body>あいう</body></html>"
#define  RESULT_STRING "<html><head></head><body text=\"#ff0000\">あいう</body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get058;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}


char *test_chxj_serf_get059(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "body    { background-color:#ff0000 }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_body_tag_with_css_002()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body>あいう</body></html>"
#define  RESULT_STRING "<html><head></head><body bgcolor=\"#ff0000\">あいう</body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get059;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}

void test_jhtml_body_tag_with_css_003()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body style=\"color:#ffff00\">あいう</body></html>"
#define  RESULT_STRING "<html><head></head><body text=\"#ffff00\">あいう</body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get059;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_body_tag_with_css_004()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body style=\"background-color:#ffff00\">あいう</body></html>"
#define  RESULT_STRING "<html><head></head><body bgcolor=\"#ffff00\">あいう</body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get059;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get060(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:link  { color:#ff0000 }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_body_tag_with_css_005()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body>あいう</body></html>"
#define  RESULT_STRING "<html><head></head><body link=\"#ff0000\">あいう</body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get060;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get062(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { color:#ff0000 }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_body_tag_with_css_006()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body>あいう</body></html>"
#define  RESULT_STRING "<html><head></head><body vlink=\"#ff0000\">あいう</body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get062;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}



/*===========================================================================*/
/* font tag with CSS                                                         */
/*===========================================================================*/
char *test_chxj_serf_get065(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "font    { color:#ff0000 }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_font_tag_with_css_001()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><font>あいう</font></html>"
#define  RESULT_STRING "<html><head></head><font color=\"#ff0000\">あいう</font></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get065;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_font_tag_with_css_002()
{
#define  TEST_STRING "<html><head>" \
                     "</head><font style=\"color:#ff0000;\">あいう</font></html>"
#define  RESULT_STRING "<html><head></head><font color=\"#ff0000\">あいう</font></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get065;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}


/*===========================================================================*/
/* form tag with CSS                                                         */
/*===========================================================================*/
char *test_chxj_serf_get073(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "form    { color:#ff0000 }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_form_tag_with_css_001()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><form>あいう</form></html>"
#define  RESULT_STRING "<html><head></head><form><font color=\"#ff0000\">あいう</font></form></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get073;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_form_tag_with_css_002()
{
#define  TEST_STRING "<html><head>" \
                     "</head><form style=\"color:#ff0000\">あいう</form></html>"
#define  RESULT_STRING "<html><head></head><form><font color=\"#ff0000\">あいう</font></form></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get073;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}

char *test_chxj_serf_get074(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "form    { text-align:left }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_form_tag_with_css_003()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><form>あいう</form></html>"
#define  RESULT_STRING "<html><head></head><form><div align=\"left\">あいう</div></form></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get074;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_form_tag_with_css_004()
{
#define  TEST_STRING "<html><head>" \
                     "</head><form style=\"text-align:left\">あいう</form></html>"
#define  RESULT_STRING "<html><head></head><form><div align=\"left\">あいう</div></form></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get074;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}

char *test_chxj_serf_get075(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "form    { text-align:center }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_form_tag_with_css_005()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><form>あいう</form></html>"
#define  RESULT_STRING "<html><head></head><form><div align=\"center\">あいう</div></form></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get075;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_form_tag_with_css_006()
{
#define  TEST_STRING "<html><head>" \
                     "</head><form style=\"text-align:center\">あいう</form></html>"
#define  RESULT_STRING "<html><head></head><form><div align=\"center\">あいう</div></form></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get075;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}

char *test_chxj_serf_get076(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "form    { text-align:right }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_form_tag_with_css_007()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><form>あいう</form></html>"
#define  RESULT_STRING "<html><head></head><form><div align=\"right\">あいう</div></form></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get076;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_form_tag_with_css_008()
{
#define  TEST_STRING "<html><head>" \
                     "</head><form style=\"text-align:right\">あいう</form></html>"
#define  RESULT_STRING "<html><head></head><form><div align=\"right\">あいう</div></form></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get076;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}



/*===========================================================================*/
/* input tag with CSS                                                        */
/*===========================================================================*/
char *test_chxj_serf_get080(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "input   { -wap-input-format: &quot;*&lt;ja:h&gt;&quot; }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_input_tag_with_css_001()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><form><input type=\"text\"></form></html>"
#define  RESULT_STRING "<html><head></head><form><input type=\"text\" mode=\"hiragana\"></form></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get080;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_input_tag_with_css_002()
{
#define  TEST_STRING "<html><head>" \
                     "</head><form><input type=\"text\" style=\"-wap-input-format: &quot;*&lt;ja:h&gt;&quot;\"></form></html>"
#define  RESULT_STRING "<html><head></head><form><input type=\"text\" mode=\"hiragana\"></form></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get080;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get081(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "input   { -wap-input-format: &quot;*&lt;ja:hk&gt;&quot; }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_input_tag_with_css_003()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><form><input type=\"text\"></form></html>"
#define  RESULT_STRING "<html><head></head><form><input type=\"text\" mode=\"hankakukana\"></form></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get081;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_input_tag_with_css_004()
{
#define  TEST_STRING "<html><head>" \
                     "</head><form><input type=\"text\" style=\"-wap-input-format: &quot;*&lt;ja:hk&gt;&quot;\"></form></html>"
#define  RESULT_STRING "<html><head></head><form><input type=\"text\" mode=\"hankakukana\"></form></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get081;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get082(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "input   { -wap-input-format: &quot;*&lt;ja:en&gt;&quot; }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_input_tag_with_css_005()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><form><input type=\"text\"></form></html>"
#define  RESULT_STRING "<html><head></head><form><input type=\"text\" mode=\"alphabet\"></form></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get082;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_input_tag_with_css_006()
{
#define  TEST_STRING "<html><head>" \
                     "</head><form><input type=\"text\" style=\"-wap-input-format: &quot;*&lt;ja:en&gt;&quot;\"></form></html>"
#define  RESULT_STRING "<html><head></head><form><input type=\"text\" mode=\"alphabet\"></form></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get082;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get083(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "input   { -wap-input-format: &quot;*&lt;ja:n&gt;&quot;; }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_input_tag_with_css_007()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><form><input type=\"text\"></form></html>"
#define  RESULT_STRING "<html><head></head><form><input type=\"text\" mode=\"numeric\"></form></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get083;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_input_tag_with_css_008()
{
#define  TEST_STRING "<html><head>" \
                     "</head><form><input type=\"text\" style=\"-wap-input-format: &quot;*&lt;ja:n&gt;&quot;;\"></form></html>"
#define  RESULT_STRING "<html><head></head><form><input type=\"text\" mode=\"numeric\"></form></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get083;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}


/*===========================================================================*/
/* center tag with CSS                                                       */
/*===========================================================================*/
char *test_chxj_serf_get090(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "center  { color:rgb(100%,0%,100%)}\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_center_tag_with_css_001()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><center>あいうえお</center></body></html>"
#define  RESULT_STRING "<html><head></head><body><center><font color=\"#ff00ff\">あいうえお</font></center></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get090;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_center_tag_with_css_002()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><center style=\"color:#ff0000\">あいうえお</center></body></html>"
#define  RESULT_STRING "<html><head></head><body><center><font color=\"#ff0000\">あいうえお</font></center></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get090;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get091(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "center  { font-size:xx-small}\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_center_tag_with_css_003()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><center>あいうえお</center></body></html>"
#define  RESULT_STRING "<html><head></head><body><center>あいうえお</center></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get091;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_center_tag_with_css_004()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><center style=\"font-size:xx-small\">あいうえお</center></body></html>"
#define  RESULT_STRING "<html><head></head><body><center>あいうえお</center></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get091;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}



/*===========================================================================*/
/* hr tag with CSS                                                           */
/*===========================================================================*/
char *test_chxj_serf_get100(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "hr      { border-style:solid }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_hr_tag_with_css_001()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><hr>あいう</body></html>"
#define  RESULT_STRING "<html><head></head><body><hr noshade>あいう</body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get100;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_hr_tag_with_css_002()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><hr style=\"border-style:solid\">あいう</body></html>"
#define  RESULT_STRING "<html><head></head><body><hr noshade>あいう</body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get100;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}


char *test_chxj_serf_get101(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "hr      { height:10px }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_hr_tag_with_css_003()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><hr>あいう</body></html>"
#define  RESULT_STRING "<html><head></head><body><hr size=\"10\">あいう</body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get101;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_hr_tag_with_css_004()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><hr style=\"height:10px\">あいう</body></html>"
#define  RESULT_STRING "<html><head></head><body><hr size=\"10\">あいう</body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get101;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}

char *test_chxj_serf_get102(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "hr      { width:10px }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_hr_tag_with_css_005()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><hr>あいう</body></html>"
#define  RESULT_STRING "<html><head></head><body><hr width=\"10\">あいう</body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get102;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_hr_tag_with_css_006()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><hr style=\"width:10px\">あいう</body></html>"
#define  RESULT_STRING "<html><head></head><body><hr width=\"10\">あいう</body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get102;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}

char *test_chxj_serf_get103(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "hr      { width:80% }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_hr_tag_with_css_007()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><hr>あいう</body></html>"
#define  RESULT_STRING "<html><head></head><body><hr width=\"80%\">あいう</body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get103;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_hr_tag_with_css_008()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><hr style=\"width:10%\">あいう</body></html>"
#define  RESULT_STRING "<html><head></head><body><hr width=\"10%\">あいう</body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get103;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}



/*---------------------------------------------------------------------------*/
/* img tag with CSS                                                          */
/*---------------------------------------------------------------------------*/
char *test_chxj_serf_get110(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "img     { height:80% }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_img_tag_with_css_001()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><img>あいう</body></html>"
#define  RESULT_STRING "<html><head></head><body><img height=\"80%\">あいう</body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get110;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_img_tag_with_css_002()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><img style=\"height:10%\">あいう</body></html>"
#define  RESULT_STRING "<html><head></head><body><img height=\"10%\">あいう</body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get110;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get111(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "img     { width:80% }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_img_tag_with_css_003()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><img>あいう</body></html>"
#define  RESULT_STRING "<html><head></head><body><img width=\"80%\">あいう</body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get111;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_img_tag_with_css_004()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><img style=\"width:10%\">あいう</body></html>"
#define  RESULT_STRING "<html><head></head><body><img width=\"10%\">あいう</body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get111;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get112(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "img     { vertical-align:top }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_img_tag_with_css_005()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><img>あいう</body></html>"
#define  RESULT_STRING "<html><head></head><body><img align=\"top\">あいう</body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get112;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_img_tag_with_css_006()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><img style=\"vertical-align:top\">あいう</body></html>"
#define  RESULT_STRING "<html><head></head><body><img align=\"top\">あいう</body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get112;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get113(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "img     { vertical-align:middle }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_img_tag_with_css_007()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><img>あいう</body></html>"
#define  RESULT_STRING "<html><head></head><body><img align=\"middle\">あいう</body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get113;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_img_tag_with_css_008()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><img style=\"vertical-align:middle\">あいう</body></html>"
#define  RESULT_STRING "<html><head></head><body><img align=\"middle\">あいう</body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get113;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get114(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "img     { vertical-align:bottom }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_img_tag_with_css_009()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><img>あいう</body></html>"
#define  RESULT_STRING "<html><head></head><body><img align=\"bottom\">あいう</body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get114;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_img_tag_with_css_010()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><img style=\"vertical-align:bottom\">あいう</body></html>"
#define  RESULT_STRING "<html><head></head><body><img align=\"bottom\">あいう</body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get114;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}


/*===========================================================================*/
/* div tag with CSS                                                          */
/*===========================================================================*/
char *test_chxj_serf_get120(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "div     { text-align:left }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_div_tag_with_css_001()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><div>あいう</div></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"left\">あいう</div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get120;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_div_tag_with_css_002()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><div style=\"text-align:left\">あいう</div></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"left\">あいう</div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get120;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}

char *test_chxj_serf_get121(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "div     { text-align:center }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_div_tag_with_css_003()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><div>あいう</div></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"center\">あいう</div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get121;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_div_tag_with_css_004()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><div style=\"text-align:center\">あいう</div></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"center\">あいう</div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get121;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get122(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "div     { text-align:right }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_div_tag_with_css_005()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><div>あいう</div></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"right\">あいう</div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get122;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_div_tag_with_css_006()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><div style=\"text-align:right\">あいう</div></body></html>"
#define  RESULT_STRING "<html><head></head><body><div align=\"right\">あいう</div></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get122;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get123(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "div     { color:#ff0000 }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_div_tag_with_css_007()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><div>あいう</div></body></html>"
#define  RESULT_STRING "<html><head></head><body><font color=\"#ff0000\">あいう</font></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get123;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_div_tag_with_css_008()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><div style=\"color:#ff0000\">あいう</div></body></html>"
#define  RESULT_STRING "<html><head></head><body><font color=\"#ff0000\">あいう</font></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get123;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get124(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "div     { text-decoration:blink }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_div_tag_with_css_009()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><div>あいう</div></body></html>"
#define  RESULT_STRING "<html><head></head><body><blink>あいう</blink></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get124;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_div_tag_with_css_010()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><div style=\"text-decoration:blink\">あいう</div></body></html>"
#define  RESULT_STRING "<html><head></head><body><blink>あいう</blink></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get124;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get125(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "div     { display:-wap-marquee }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_div_tag_with_css_011()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><div>あいう</div></body></html>"
#define  RESULT_STRING "<html><head></head><body><marquee>あいう</marquee></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get125;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_div_tag_with_css_012()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><div style=\"display:-wap-marquee\">あいう</div></body></html>"
#define  RESULT_STRING "<html><head></head><body><marquee>あいう</marquee></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get125;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get126(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "div     { display:-wap-marquee;\n"
                     "          -wap-marquee-style: scroll; }\n";

  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_div_tag_with_css_013()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><div>あいう</div></body></html>"
#define  RESULT_STRING "<html><head></head><body><marquee behavior=\"scroll\">あいう</marquee></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get126;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_div_tag_with_css_014()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><div style=\"display:-wap-marquee;-wap-marquee-style:scroll;\">あいう</div></body></html>"
#define  RESULT_STRING "<html><head></head><body><marquee behavior=\"scroll\">あいう</marquee></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get126;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get127(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "div     { display:-wap-marquee;\n"
                     "          -wap-marquee-style: slide; }\n";

  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_div_tag_with_css_015()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><div>あいう</div></body></html>"
#define  RESULT_STRING "<html><head></head><body><marquee behavior=\"slide\">あいう</marquee></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get127;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_div_tag_with_css_016()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><div style=\"display:-wap-marquee;-wap-marquee-style:slide;\">あいう</div></body></html>"
#define  RESULT_STRING "<html><head></head><body><marquee behavior=\"slide\">あいう</marquee></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get127;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get128(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "div     { display:-wap-marquee;\n"
                     "          -wap-marquee-style: alternate }\n";

  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_div_tag_with_css_017()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><div>あいう</div></body></html>"
#define  RESULT_STRING "<html><head></head><body><marquee behavior=\"alternate\">あいう</marquee></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get128;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_div_tag_with_css_018()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><div style=\"display:-wap-marquee;-wap-marquee-style:alternate\">あいう</div></body></html>"
#define  RESULT_STRING "<html><head></head><body><marquee behavior=\"alternate\">あいう</marquee></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get128;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get129(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "div     { display:-wap-marquee;\n"
                     "          -wap-marquee-style: alternate;-wap-marquee-dir:ltr }\n";

  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_div_tag_with_css_019()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><div>あいう</div></body></html>"
#define  RESULT_STRING "<html><head></head><body><marquee behavior=\"alternate\" direction=\"right\">あいう</marquee></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get129;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_div_tag_with_css_020()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><div style=\"display:-wap-marquee;-wap-marquee-style:alternate;-wap-marquee-dir:ltr\">あいう</div></body></html>"
#define  RESULT_STRING "<html><head></head><body><marquee behavior=\"alternate\" direction=\"right\">あいう</marquee></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get129;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get130(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "div     { display:-wap-marquee;\n"
                     "          -wap-marquee-style: alternate;-wap-marquee-dir:rtl }\n";

  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_div_tag_with_css_021()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><div>あいう</div></body></html>"
#define  RESULT_STRING "<html><head></head><body><marquee behavior=\"alternate\" direction=\"left\">あいう</marquee></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get130;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_div_tag_with_css_022()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><div style=\"display:-wap-marquee;-wap-marquee-style:alternate;-wap-marquee-dir:rtl\">あいう</div></body></html>"
#define  RESULT_STRING "<html><head></head><body><marquee behavior=\"alternate\" direction=\"left\">あいう</marquee></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get130;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get131(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "div     { display:-wap-marquee;\n"
                     "          -wap-marquee-style: alternate;-wap-marquee-dir:rtl;-wap-marquee-loop:1 }\n";

  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_div_tag_with_css_023()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><div>あいう</div></body></html>"
#define  RESULT_STRING "<html><head></head><body><marquee behavior=\"alternate\" direction=\"left\" loop=\"1\">あいう</marquee></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get131;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_div_tag_with_css_024()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><div style=\"display:-wap-marquee;-wap-marquee-style:alternate;-wap-marquee-dir:rtl;-wap-marquee-loop:1\">あいう</div></body></html>"
#define  RESULT_STRING "<html><head></head><body><marquee behavior=\"alternate\" direction=\"left\" loop=\"1\">あいう</marquee></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get131;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get132(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "div     { display:-wap-marquee;\n"
                     "          -wap-marquee-style: alternate;-wap-marquee-dir:rtl;-wap-marquee-loop:16 }\n";

  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_div_tag_with_css_025()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><div>あいう</div></body></html>"
#define  RESULT_STRING "<html><head></head><body><marquee behavior=\"alternate\" direction=\"left\" loop=\"16\">あいう</marquee></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get132;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_div_tag_with_css_026()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><div style=\"display:-wap-marquee;-wap-marquee-style:alternate;-wap-marquee-dir:rtl;-wap-marquee-loop:16\">あいう</div></body></html>"
#define  RESULT_STRING "<html><head></head><body><marquee behavior=\"alternate\" direction=\"left\" loop=\"16\">あいう</marquee></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get132;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get133(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "div     { display:-wap-marquee;\n"
                     "          -wap-marquee-style: alternate;-wap-marquee-dir:rtl;-wap-marquee-loop:infinite }\n";

  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_div_tag_with_css_027()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><div>あいう</div></body></html>"
#define  RESULT_STRING "<html><head></head><body><marquee behavior=\"alternate\" direction=\"left\" loop=\"16\">あいう</marquee></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get133;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_div_tag_with_css_028()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><div style=\"display:-wap-marquee;-wap-marquee-style:alternate;-wap-marquee-dir:rtl;-wap-marquee-loop:infinite;\">あいう</div></body></html>"
#define  RESULT_STRING "<html><head></head><body><marquee behavior=\"alternate\" direction=\"left\" loop=\"16\">あいう</marquee></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get133;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get134(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "div     { font-size:xx-small; }\n";

  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_div_tag_with_css_029()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><div>あいう</div></body></html>"
#define  RESULT_STRING "<html><head></head><body><font size=\"1\">あいう</font></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get134;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_div_tag_with_css_030()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><div style=\"font-size:xx-small;\">あいう</div></body></html>"
#define  RESULT_STRING "<html><head></head><body><font size=\"1\">あいう</font></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get134;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}

char *test_chxj_serf_get135(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "div     { font-size:x-small; }\n";

  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_div_tag_with_css_031()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><div>あいう</div></body></html>"
#define  RESULT_STRING "<html><head></head><body><font size=\"2\">あいう</font></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get135;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_div_tag_with_css_032()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><div style=\"font-size:x-small;\">あいう</div></body></html>"
#define  RESULT_STRING "<html><head></head><body><font size=\"2\">あいう</font></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get135;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}

char *test_chxj_serf_get136(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "div     { font-size:small; }\n";

  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_div_tag_with_css_033()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><div>あいう</div></body></html>"
#define  RESULT_STRING "<html><head></head><body><font size=\"3\">あいう</font></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get136;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_div_tag_with_css_034()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><div style=\"font-size:small;\">あいう</div></body></html>"
#define  RESULT_STRING "<html><head></head><body><font size=\"3\">あいう</font></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get136;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get137(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "div     { font-size:medium; }\n";

  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_div_tag_with_css_035()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><div>あいう</div></body></html>"
#define  RESULT_STRING "<html><head></head><body><font size=\"4\">あいう</font></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get137;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_div_tag_with_css_036()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><div style=\"font-size:medium;\">あいう</div></body></html>"
#define  RESULT_STRING "<html><head></head><body><font size=\"4\">あいう</font></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get137;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get138(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "div     { font-size:large; }\n";

  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_div_tag_with_css_037()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><div>あいう</div></body></html>"
#define  RESULT_STRING "<html><head></head><body><font size=\"5\">あいう</font></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get138;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_div_tag_with_css_038()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><div style=\"font-size:large;\">あいう</div></body></html>"
#define  RESULT_STRING "<html><head></head><body><font size=\"5\">あいう</font></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get138;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get139(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "div     { font-size:x-large; }\n";

  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_div_tag_with_css_039()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><div>あいう</div></body></html>"
#define  RESULT_STRING "<html><head></head><body><font size=\"6\">あいう</font></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get139;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_div_tag_with_css_040()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><div style=\"font-size:x-large;\">あいう</div></body></html>"
#define  RESULT_STRING "<html><head></head><body><font size=\"6\">あいう</font></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get139;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get140(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "div     { font-size:xx-large; }\n";

  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_div_tag_with_css_041()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><div>あいう</div></body></html>"
#define  RESULT_STRING "<html><head></head><body><font size=\"7\">あいう</font></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get140;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_div_tag_with_css_042()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><div style=\"font-size:xx-large;\">あいう</div></body></html>"
#define  RESULT_STRING "<html><head></head><body><font size=\"7\">あいう</font></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get140;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}



/*===========================================================================*/
/* dt tag with CSS                                                           */
/*===========================================================================*/
char *test_chxj_serf_get150(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "dt      { font-size:xx-small; }\n";

  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_dt_tag_with_css_001()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><dl><dt>あいう</dt></dl></body></html>"
#define  RESULT_STRING "<html><head></head><body><dl><dt><font size=\"1\">あいう</font></dl></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get150;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_dt_tag_with_css_002()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><dl><dt style=\"font-size:xx-small;\">あいう</dt></dl></body></html>"
#define  RESULT_STRING "<html><head></head><body><dl><dt><font size=\"1\">あいう</font></dl></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get150;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get151(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "dt      { font-size:x-small; }\n";

  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_dt_tag_with_css_003()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><dl><dt>あいう</dt></dl></body></html>"
#define  RESULT_STRING "<html><head></head><body><dl><dt><font size=\"2\">あいう</font></dl></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get151;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_dt_tag_with_css_004()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><dl><dt style=\"font-size:x-small;\">あいう</dt></dl></body></html>"
#define  RESULT_STRING "<html><head></head><body><dl><dt><font size=\"2\">あいう</font></dl></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get151;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get152(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "dt      { font-size:small; }\n";

  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_dt_tag_with_css_005()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><dl><dt>あいう</dt></dl></body></html>"
#define  RESULT_STRING "<html><head></head><body><dl><dt><font size=\"3\">あいう</font></dl></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get152;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_dt_tag_with_css_006()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><dl><dt style=\"font-size:small;\">あいう</dt></dl></body></html>"
#define  RESULT_STRING "<html><head></head><body><dl><dt><font size=\"3\">あいう</font></dl></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get152;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get153(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "dt      { font-size:medium; }\n";

  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_dt_tag_with_css_007()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><dl><dt>あいう</dt></dl></body></html>"
#define  RESULT_STRING "<html><head></head><body><dl><dt><font size=\"4\">あいう</font></dl></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get153;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_dt_tag_with_css_008()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><dl><dt style=\"font-size:medium;\">あいう</dt></dl></body></html>"
#define  RESULT_STRING "<html><head></head><body><dl><dt><font size=\"4\">あいう</font></dl></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get153;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get154(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "dt      { font-size:large; }\n";

  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_dt_tag_with_css_009()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><dl><dt>あいう</dt></dl></body></html>"
#define  RESULT_STRING "<html><head></head><body><dl><dt><font size=\"5\">あいう</font></dl></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get154;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_dt_tag_with_css_010()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><dl><dt style=\"font-size:large;\">あいう</dt></dl></body></html>"
#define  RESULT_STRING "<html><head></head><body><dl><dt><font size=\"5\">あいう</font></dl></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get154;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get155(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "dt      { font-size:x-large; }\n";

  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_dt_tag_with_css_011()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><dl><dt>あいう</dt></dl></body></html>"
#define  RESULT_STRING "<html><head></head><body><dl><dt><font size=\"6\">あいう</font></dl></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get155;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_dt_tag_with_css_012()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><dl><dt style=\"font-size:x-large;\">あいう</dt></dl></body></html>"
#define  RESULT_STRING "<html><head></head><body><dl><dt><font size=\"6\">あいう</font></dl></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get155;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get156(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "dt      { font-size:xx-large; }\n";

  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_dt_tag_with_css_013()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><dl><dt>あいう</dt></dl></body></html>"
#define  RESULT_STRING "<html><head></head><body><dl><dt><font size=\"7\">あいう</font></dl></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get156;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_dt_tag_with_css_014()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><dl><dt style=\"font-size:xx-large;\">あいう</dt></dl></body></html>"
#define  RESULT_STRING "<html><head></head><body><dl><dt><font size=\"7\">あいう</font></dl></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get156;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}





/*===========================================================================*/
/* blockquote tag with CSS                                                   */
/*===========================================================================*/
char *test_chxj_serf_get160(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "blockquote { color: #ff0000 }\n";

  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_blockquote_tag_with_css_001()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><blockquote>あいう</blockquote></body></html>"
#define  RESULT_STRING "<html><head></head><body><blockquote><font color=\"#ff0000\">あいう</font></blockquote></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get160;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_blockquote_tag_with_css_002()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><blockquote style=\"color:#ff0000\">あいう</blockquote></body></html>"
#define  RESULT_STRING "<html><head></head><body><blockquote><font color=\"#ff0000\">あいう</font></blockquote></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get160;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}


char *test_chxj_serf_get161(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "blockquote { font-size: xx-small }\n";

  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_blockquote_tag_with_css_003()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><blockquote>あいう</blockquote></body></html>"
#define  RESULT_STRING "<html><head></head><body><blockquote><font size=\"1\">あいう</font></blockquote></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get161;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_blockquote_tag_with_css_004()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><blockquote style=\"font-size:xx-small\">あいう</blockquote></body></html>"
#define  RESULT_STRING "<html><head></head><body><blockquote><font size=\"1\">あいう</font></blockquote></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get161;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get162(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "blockquote { font-size: x-small }\n";

  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_blockquote_tag_with_css_005()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><blockquote>あいう</blockquote></body></html>"
#define  RESULT_STRING "<html><head></head><body><blockquote><font size=\"2\">あいう</font></blockquote></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get162;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_blockquote_tag_with_css_006()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><blockquote style=\"font-size:x-small\">あいう</blockquote></body></html>"
#define  RESULT_STRING "<html><head></head><body><blockquote><font size=\"2\">あいう</font></blockquote></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get162;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get163(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "blockquote { font-size: small }\n";

  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_blockquote_tag_with_css_007()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><blockquote>あいう</blockquote></body></html>"
#define  RESULT_STRING "<html><head></head><body><blockquote><font size=\"3\">あいう</font></blockquote></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get163;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_blockquote_tag_with_css_008()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><blockquote style=\"font-size:small\">あいう</blockquote></body></html>"
#define  RESULT_STRING "<html><head></head><body><blockquote><font size=\"3\">あいう</font></blockquote></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get163;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get164(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "blockquote { font-size: medium }\n";

  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_blockquote_tag_with_css_009()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><blockquote>あいう</blockquote></body></html>"
#define  RESULT_STRING "<html><head></head><body><blockquote><font size=\"4\">あいう</font></blockquote></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get164;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_blockquote_tag_with_css_010()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><blockquote style=\"font-size:medium\">あいう</blockquote></body></html>"
#define  RESULT_STRING "<html><head></head><body><blockquote><font size=\"4\">あいう</font></blockquote></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get164;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get165(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "blockquote { font-size: large }\n";

  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_blockquote_tag_with_css_011()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><blockquote>あいう</blockquote></body></html>"
#define  RESULT_STRING "<html><head></head><body><blockquote><font size=\"5\">あいう</font></blockquote></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get165;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_blockquote_tag_with_css_012()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><blockquote style=\"font-size:large\">あいう</blockquote></body></html>"
#define  RESULT_STRING "<html><head></head><body><blockquote><font size=\"5\">あいう</font></blockquote></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get165;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get166(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "blockquote { font-size: x-large }\n";

  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_blockquote_tag_with_css_013()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><blockquote>あいう</blockquote></body></html>"
#define  RESULT_STRING "<html><head></head><body><blockquote><font size=\"6\">あいう</font></blockquote></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get166;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_blockquote_tag_with_css_014()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><blockquote style=\"font-size:x-large\">あいう</blockquote></body></html>"
#define  RESULT_STRING "<html><head></head><body><blockquote><font size=\"6\">あいう</font></blockquote></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get166;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get167(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "blockquote { font-size: xx-large }\n";

  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_blockquote_tag_with_css_015()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><blockquote>あいう</blockquote></body></html>"
#define  RESULT_STRING "<html><head></head><body><blockquote><font size=\"7\">あいう</font></blockquote></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get167;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_blockquote_tag_with_css_016()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><blockquote style=\"font-size:xx-large\">あいう</blockquote></body></html>"
#define  RESULT_STRING "<html><head></head><body><blockquote><font size=\"7\">あいう</font></blockquote></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get167;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}








/*===========================================================================*/
/* dir tag with CSS                                                          */
/*===========================================================================*/
char *test_chxj_serf_get180(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "dir { color: #ff0000 }\n";

  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_dir_tag_with_css_001()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><dir>あいう</dir></body></html>"
#define  RESULT_STRING "<html><head></head><body><dir><font color=\"#ff0000\">あいう</font></dir></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get180;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_dir_tag_with_css_002()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><dir style=\"color:#ff0000\">あいう</dir></body></html>"
#define  RESULT_STRING "<html><head></head><body><dir><font color=\"#ff0000\">あいう</font></dir></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get180;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}

char *test_chxj_serf_get181(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "dir { list-style-type:disc }\n";
/*
list-style-type      =>  disc | circle | square => type="disc|circle|square"に変換
*/

  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_dir_tag_with_css_003()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><dir>あいう</dir></body></html>"
#define  RESULT_STRING "<html><head></head><body><dir>あいう</dir></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get181;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_dir_tag_with_css_004()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><dir style=\"list-style-type:disc\">あいう</dir></body></html>"
#define  RESULT_STRING "<html><head></head><body><dir>あいう</dir></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get181;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}


char *test_chxj_serf_get182(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "dir { list-style-type:circle }\n";
/*
list-style-type      =>  circle | circle | square => type="circle|circle|square"に変換
*/

  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_dir_tag_with_css_005()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><dir>あいう</dir></body></html>"
#define  RESULT_STRING "<html><head></head><body><dir>あいう</dir></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get182;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_dir_tag_with_css_006()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><dir style=\"list-style-type:circle\">あいう</dir></body></html>"
#define  RESULT_STRING "<html><head></head><body><dir>あいう</dir></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get182;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}



char *test_chxj_serf_get183(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "dir { list-style-type:square }\n";
/*
list-style-type      =>  square | square | square => type="square|square|square"に変換
*/

  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_dir_tag_with_css_007()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><dir>あいう</dir></body></html>"
#define  RESULT_STRING "<html><head></head><body><dir>あいう</dir></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get183;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_dir_tag_with_css_008()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><dir style=\"list-style-type:square\">あいう</dir></body></html>"
#define  RESULT_STRING "<html><head></head><body><dir>あいう</dir></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get183;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}



char *test_chxj_serf_get184(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "dir { font-size:xx-small }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_dir_tag_with_css_009()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><dir>あいう</dir></body></html>"
#define  RESULT_STRING "<html><head></head><body><dir><font size=\"1\">あいう</font></dir></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get184;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_dir_tag_with_css_010()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><dir style=\"font-size:xx-small\">あいう</dir></body></html>"
#define  RESULT_STRING "<html><head></head><body><dir><font size=\"1\">あいう</font></dir></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get184;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}



char *test_chxj_serf_get185(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "dir { font-size:x-small }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_dir_tag_with_css_011()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><dir>あいう</dir></body></html>"
#define  RESULT_STRING "<html><head></head><body><dir><font size=\"2\">あいう</font></dir></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get185;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_dir_tag_with_css_012()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><dir style=\"font-size:x-small\">あいう</dir></body></html>"
#define  RESULT_STRING "<html><head></head><body><dir><font size=\"2\">あいう</font></dir></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get185;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}



char *test_chxj_serf_get186(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "dir { font-size:small }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_dir_tag_with_css_013()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><dir>あいう</dir></body></html>"
#define  RESULT_STRING "<html><head></head><body><dir><font size=\"3\">あいう</font></dir></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get186;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_dir_tag_with_css_014()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><dir style=\"font-size:small\">あいう</dir></body></html>"
#define  RESULT_STRING "<html><head></head><body><dir><font size=\"3\">あいう</font></dir></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get186;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get187(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "dir { font-size:medium }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_dir_tag_with_css_015()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><dir>あいう</dir></body></html>"
#define  RESULT_STRING "<html><head></head><body><dir><font size=\"4\">あいう</font></dir></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get187;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_dir_tag_with_css_016()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><dir style=\"font-size:medium\">あいう</dir></body></html>"
#define  RESULT_STRING "<html><head></head><body><dir><font size=\"4\">あいう</font></dir></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get187;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
char *test_chxj_serf_get188(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "dir { font-size:large }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_dir_tag_with_css_017()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><dir>あいう</dir></body></html>"
#define  RESULT_STRING "<html><head></head><body><dir><font size=\"5\">あいう</font></dir></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get188;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_dir_tag_with_css_018()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><dir style=\"font-size:large\">あいう</dir></body></html>"
#define  RESULT_STRING "<html><head></head><body><dir><font size=\"5\">あいう</font></dir></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get188;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}


char *test_chxj_serf_get189(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "dir { font-size:x-large }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_dir_tag_with_css_019()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><dir>あいう</dir></body></html>"
#define  RESULT_STRING "<html><head></head><body><dir><font size=\"6\">あいう</font></dir></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get189;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_dir_tag_with_css_020()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><dir style=\"font-size:x-large\">あいう</dir></body></html>"
#define  RESULT_STRING "<html><head></head><body><dir><font size=\"6\">あいう</font></dir></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get189;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}


char *test_chxj_serf_get190(request_rec *r, apr_pool_t *ppool, const char *uri_path, int ss, apr_size_t *len)
{
  static char *css = "a:focus { display: none }\n"
                     "a:link  { display: none }\n"
                     "a       { display: none }\n"
                     "hr      { display: none }\n"
                     "a:visited { display:none }\n"
                     "dir { font-size:xx-large }\n";
  *len = strlen(css);
  call_check = 1;
  return css;
}
void test_jhtml_dir_tag_with_css_021()
{
#define  TEST_STRING "<html><head><link rel=\"stylesheet\" href=\"http://localhost/a.css\"  type=\"text/css\" />" \
                     "</head><body><dir>あいう</dir></body></html>"
#define  RESULT_STRING "<html><head></head><body><dir><font size=\"7\">あいう</font></dir></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get190;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jhtml_dir_tag_with_css_022()
{
#define  TEST_STRING "<html><head>" \
                     "</head><body><dir style=\"font-size:xx-large\">あいう</dir></body></html>"
#define  RESULT_STRING "<html><head></head><body><dir><font size=\"7\">あいう</font></dir></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;
  chxj_serf_get = test_chxj_serf_get190;
  call_check = 0;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;
  entry.action |= CONVRULE_CSS_ON_BIT;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);
  CU_ASSERT(call_check == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*
 * vim:ts=2 et
 */
