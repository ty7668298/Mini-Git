# READ_ME

### 测试用例设计

1. 在当前文件夹中保存 f1.txt  f2.txt  f3.txt

   f1.txt 的内容

   ```
   1
   2
   ```

   f2.txt 的内容

   ```
   3
   4
   ```

   f3.txt 的内容

   ```
   5
   6
   ```

2. 两次选择Adding files，添加 f1.txt 和 f2.txt

3. Commit一次，这时候 .minigit 文件夹中应该有 f1_00.txt 和 f2_00.txt

4. 修改 f1.txt 为如下内容

   ```
   7
   8
   ```

5. 修改 f2.txt 为如下内容

   ```
   9
   10
   ```

6. 选择Removing files，删除 f2.txt

7. 选择Adding files，添加 f3.txt

8. 再次Commit

9. 这时候 .minigit 中应该有 f1_00.txt f2_00.txt f1_01.txt f3_00.txt，但不会有 f2_01.txt，因为 f2.txt 被删除了

10. 最后选择Check out，并输入commitNumber为 0，这时候当前文件夹下的 f1.txt 和 f2.txt 应该会恢复最初始的值