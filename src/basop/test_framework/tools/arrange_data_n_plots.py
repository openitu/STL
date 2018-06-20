import xlrd
import xlsxwriter
import os
import shutil

def excel_op(s, i, offset):

   filename='basop_ip_op_plot.xlsx'
   sheet_name='Sheet1'
   book = xlrd.open_workbook(str(filename))

   sheet = book.sheet_by_name(sheet_name)
   workbook = xlsxwriter.Workbook(basops[i]+".xlsx")

   worksheet0 = workbook.add_worksheet('test setup')
   worksheet0.insert_image('H5', './TEst_setup.JPG')  

   worksheet = workbook.add_worksheet('random')

   line = 1
   row_count = int(1002+offset)
   col_count = int(sheet.ncols)
   for col in range(0, int(col_count)):
       worksheet.write(0,col,str(sheet.cell(0, col).value))
   for row in range(3+offset, int(row_count)):
       for col in range(0, int(col_count)):
           if str(sheet.cell(row, col).value) == '-00':
               worksheet.write(line,col,str(sheet.cell(1, col).value))
	   else:
               worksheet.write(line,col,str(sheet.cell(row, col).value))
       line = line+1
   worksheet.insert_image('H5', './plots/pat2'+basops[i]+'.png')  

   worksheet1 = workbook.add_worksheet('sweep')

   line = 1
   row_count = int(2002+offset)
   col_count = int(sheet.ncols)
   for col in range(0, int(col_count)):
       worksheet1.write(0,col,str(sheet.cell(0, col).value))
   for row in range(1003+offset, int(row_count)):
       for col in range(0, int(col_count)):
           if str(sheet.cell(row, col).value) == '-00':
               worksheet1.write(line,col,str(sheet.cell(1, col).value))
           else:
               worksheet1.write(line,col,str(sheet.cell(row, col).value))
       line = line+1
   worksheet1.insert_image('H5', './plots/pat1'+basops[i]+'.png')  


   worksheet2 = workbook.add_worksheet('sweep_piecewise')
   line = 1
   row_count = int(3002+offset)
   col_count = int(sheet.ncols)
   for col in range(0, int(col_count)):
       worksheet2.write(0,col,str(sheet.cell(0, col).value))
   for row in range(2002+offset, int(row_count)):
       for col in range(0, int(col_count)):
           if str(sheet.cell(row, col).value) == '-00':
               worksheet2.write(line,col,str(sheet.cell(1, col).value))
           else:
               worksheet2.write(line,col,str(sheet.cell(row, col).value))
       line = line+1
   worksheet2.insert_image('H5', './plots/pat3'+basops[i]+'.png')  


   worksheet3 = workbook.add_worksheet('custom')
   line = 1
   row_count = int(4002+offset)
   col_count = int(sheet.ncols)
   for col in range(0, int(col_count)):
       worksheet3.write(0,col,str(sheet.cell(0, col).value))
   for row in range(3003+offset, int(row_count)):
       for col in range(0, int(col_count)):
           if str(sheet.cell(row, col).value) == '-00':
               worksheet3.write(line,col,str(sheet.cell(1, col).value))
           else:
               worksheet3.write(line,col,str(sheet.cell(row, col).value))
       line = line+1

   # Insert an image.
   worksheet3.insert_image('H5', './plots/pat4'+basops[i]+'.png')  

   workbook.close()
   del book


basops = ["W_mult_16_16", "W_mac_16_16", "W_msu_16_16", "W_add", "W_sub", "W_add_nosat", "W_sub_nosat", "W_shl", "W_shr", "W_shl_nosat", "W_shr_nosat", "W_lshl", "W_lshr", "W_shl_sat_l", "W_sat_l", "W_sat_m", "W_round48_L", "W_round64_L", "W_round32_s", "W_norm", "W_mult0_16_16", "W_mac0_16_16", "W_msu0_16_16", "W_mac_32_16", "W_msu_32_16", "W_mult_32_16", "W_mult_32_32", "W_mult0_32_32", "W_neg", "W_abs",  "Madd_32_16", "Madd_32_32", "Madd_32_16_r", "Madd_32_32_r", "Mpy_32_16_1", "Mpy_32_32", "Mpy_32_32_r", "Mpy_32_16_r", "Msub_32_16", "Msub_32_16_r", "Msub_32_32", "Msub_32_32_r", "CL_add", "CL_sub", "CL_msu_j", "CL_mac_j", "CL_multr_32x32", "CL_multr_32x16", "C_add", "C_sub", "C_multr", "C_scale", "CL_negate", "CL_mul_j", "C_negate", "C_mul_j", "C_mac_r", "C_msu_r", "CL_shr", "CL_shl", "C_shr", "C_shl", "CL_scale_32", "CL_scale", "CL_dscale", "CL_dscale_32", "CL_round32_16"]


for i in range(0, 67):
#for i in range(11, 13):
    excel_op(basops[i], i, 4000*i)

if not os.path.exists('./data_sheets/'):
    os.makedirs('./data_sheets/')

for i in range (0, 67):
#for i in range (11, 13):
   shutil.move(basops[i]+'.xlsx', 'data_sheets/'+basops[i]+'.xlsx')
